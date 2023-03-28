#include "Optimize.h"

#include "Command.h"
#include "Position.h"
#include "Part.h"
#include "Vector.h"

#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <string.h>
#include <vector>

namespace {
	Part removeZ( const Part& part ){
		Part result = part;
		for( size_t i=0; i<part.size(); ++i ){
			result[i].remove('Z');
		}
		return result;
	}

	struct Block {
		Block( size_t idx, const Position& pos ): startIdx(idx), endIdx(idx), startPos(pos),endPos(pos){}
		size_t startIdx = 0, endIdx = 0;
		Position startPos, endPos;
		Part part;
	};

	Block extractInitBlock( const Part& part, const Position& start ){
		Block res( 0, start );
		//Comment Block
		while( res.endIdx<part.size() && part[res.endIdx].isComment() ){ 
			res.part.append( part[res.endIdx] );
			++res.endIdx; 
		}
		//Non G-Code Block (e.g. M03)
		while( res.endIdx<part.size() && !part[res.endIdx].hasKey('G') ){ 
			res.part.append( part[res.endIdx] );
			++res.endIdx; 
		}
		res.endPos = res.part.endPosition( start );
		return res;
	}

	Block extractApproachBlock( const Part& part, size_t idx, const Position& start ){
		Block res( idx, start );
		while( idx<part.size() && ( part[idx].value('G') == "00" || (!part[idx].hasKey('X') && !part[idx].hasKey('Y')) ) ){ 
			res.part.append( part[idx] );
			++idx; 
		}
		res.endIdx = idx;
		res.endPos = res.part.endPosition( start );
		return res;
	}

	Block extractDiveBlock( const Part& part, size_t idx, const Position& start ){
		Block res( idx, start );
		while( idx<part.size() && part[idx].hasKey('Z') ){
			res.part.append( part[idx] );
			++idx;
		}
		res.endIdx = idx;
		res.endPos = res.part.endPosition( start );
		std::cout << "Detected dive-block length: " << res.part.size() << std::endl;
		return res;
	}

	Block extractTrackBlock( const Part& part, size_t idx, const Position& start ){
		Block res( idx, start );
		while( idx<part.size() && !part[idx].hasKey('Z') ){
			res.part.append( part[idx] );
			++idx;
		}
		res.endIdx = idx;
		res.endPos = res.part.endPosition( start );
		std::cout << "Detected track-block length: " << res.part.size() << std::endl;
		return res;
	}

	Block extracktCyclesBlock( const Part& part, const Part& cycle, size_t idx, const Position& start ){
		Block res( idx, start );
		while(idx<part.size()){
			std::cout << "Analyzing cycle...";
			Part candidate = removeZ( part.subPart( idx, idx+cycle.size() ) );
			if( candidate == cycle ){
				std::cout << " match detected" << std::endl;
				res.part.append( part.subPart( idx, idx+cycle.size() ) );
				idx += cycle.size();
			} else { 
				std::cout << " missmatch detected" << std::endl;
				break; 
			}
		}
		res.endIdx = idx;
		res.endPos = res.part.endPosition( start );
		return res;
	}

	double extractAngle( const Block& diveBlock ){
		auto pos = diveBlock.part[0].endPosition( diveBlock.startPos );
		double zdiff= pos.depth - diveBlock.startPos.depth;
		double length = diveBlock.part[0].pathLength( diveBlock.startPos );
		double angle = atan( zdiff / length );
		std::cout << "Detected dive-angle: " << angle/2.0/M_PI*360.0 << "°" << std::endl;
		return angle;
	}

	bool validate( const Block& dive, const Block& track ){
		size_t steps = dive.part.size()/2;
		if( track.part.size() < steps ){
			return false;
		}

		bool valid = true;
		for( size_t i=0; i<steps-1; ++i ){ // the last cmd may differ
			auto cmd = dive.part[i];
			cmd.remove('Z');
			valid = valid && cmd == track.part[i];
		}
		return valid;
	}

	std::array< Command, 2 > split( const Command& cmd, const Position& pos, double percent ){
		auto endPos = cmd.endPosition( pos );
		
		auto code = std::stoul( cmd.value('G') );
		if( code <= 1 ){
			//Linear movement
			auto dx = endPos.x-pos.x;
			auto dy = endPos.y-pos.y;
			auto dz = endPos.depth-pos.depth;
			Position splitPos( pos.x + percent*dx, pos.y + percent*dy, pos.depth + percent*dz);
			
			Command cmd1 = cmd;
			if( cmd.hasKey('X') ) cmd1.setValue('X', std::to_string( splitPos.x ) );
			if( cmd.hasKey('Y') ) cmd1.setValue('Y', std::to_string( splitPos.y ) );
			if( cmd.hasKey('Z') ) cmd1.setValue('Z', std::to_string( splitPos.depth ) );
			return {cmd1, cmd };
		} else if ( code <=3 ){
			auto i = std::stod( cmd.value('I') );
			auto j = std::stod( cmd.value('J') );
			Position center = Position( pos.x+i, pos.y+j );

			//the angle can be calculated from the scalar product
			Vector ca( center, pos );
			Vector cb( center, endPos );
			double angle = acos( (ca*cb)/ca.norm()/cb.norm() );

			//Rotate ca by (angle*percent) to find splitPos
			double a = -angle*percent;
			Vector v = ca;
			v.x = cos(a)*ca.x-sin(a)*ca.y;
			v.y = sin(a)*ca.x+cos(a)*ca.y;
			auto dz = endPos.depth-pos.depth;
			Position splitPos( center.x + v.x, center.y+v.y, pos.depth + percent*dz );

			Command cmd1 = cmd;
			if( cmd.hasKey('X') ) cmd1.setValue('X', std::to_string( splitPos.x ) );
			if( cmd.hasKey('Y') ) cmd1.setValue('Y', std::to_string( splitPos.y ) );
			if( cmd.hasKey('Z') ) cmd1.setValue('Z', std::to_string( splitPos.depth ) );

			Command cmd2 = cmd; //X,Y,Z still correct
			cmd2.setValue( 'I', std::to_string( center.x - splitPos.x ) );
			cmd2.setValue( 'J', std::to_string( center.y - splitPos.y ) );
			return {cmd1, cmd2 };
		}
		throw std::runtime_error("Not implemented");
	}

	Part buildSpiralCycle( const Block& track, double angle, double cycledepth, double startz ){
		Position pos = track.startPos;
		pos.depth = startz;

		//build dive part and track part
		Part cycle;
		size_t idx = 0;
		double depth = 0;
		while( depth > cycledepth ){
			auto cmd = track.part[idx];
			double l = cmd.pathLength( pos );
			double delta = tan(angle)*l;

			//fix numerical issue - Avoid split if l ist just a very little to long - so we use a little slighter angle
			if( fabs(depth+delta - cycledepth) < 0.0001 ){
				delta = cycledepth-depth;
			}

			if( depth+delta < cycledepth ){
				//if delta brings us to low, we have to divide the cmd path
				//calculate percentage of the path part we need for the ramp
				double percent = 1.0 + (cycledepth - ( depth+delta )) / delta;
				auto cmds = split( cmd, pos, percent );
				depth = cycledepth;
				cmds[0].setValue('Z', std::to_string( startz + depth ) );
				cycle.append( cmds[0] );
				cycle.append( cmds[1] );
				++idx;
				break;
			} else {
				depth += delta;
				cmd.setValue( 'Z', std::to_string( startz + depth ) );
				cycle.append( cmd );
				pos = cmd.endPosition( pos );
				++idx;
			}
		}
		cycle.append( track.part.subPart( idx ) );

		return cycle;
	}
}

Part optimizePart(const Part& part, const Position& start ) {
	if( part.isEmpty() ){
		std::cout << "Skipping empty part" << std::endl;
		return part;
	}
	std::cout << "Analyzing part " << part[0].toString() << std::endl; //typically a comment with the name of the part

	auto initBlock = extractInitBlock( part, start );
	auto approachBlock = extractApproachBlock( part, initBlock.endIdx, initBlock.endPos );
	auto diveBlock = extractDiveBlock( part, approachBlock.endIdx, approachBlock.endPos );
	if( diveBlock.part.size()<2 || diveBlock.endPos != diveBlock.startPos ){
		std::cout << "Discarded - invalid dive-block" << std::endl;
		return part;
	}

	auto trackBlock = extractTrackBlock( part, diveBlock.endIdx, diveBlock.endPos );
	if( trackBlock.part.isEmpty() || trackBlock.endPos != trackBlock.startPos ){
		std::cout << "Discarded - invalid track-block" << std::endl;
		return part;
	}

	if( !validate( diveBlock, trackBlock ) ){
		std::cout << "Discarded - track-block does not match dive-block" << std::endl;
		return part;
	}

	//dive + track are the complete cycle that will repeat now -> build a cycle template from both blocks
	Part cycleTemplate = removeZ( diveBlock.part );
	cycleTemplate.append( trackBlock.part );

    // build a Block containing all cycles including dive and track found above
	auto cyclesBlock = extracktCyclesBlock( part, cycleTemplate, diveBlock.startIdx, diveBlock.startPos );
	if( cyclesBlock.part.size()/cycleTemplate.size() < 2 ){
		std::cout << "Discarded - not enough cycles for optimization" << std::endl;
		return part;
	}
	
	std::cout << "Optimizing..." << std::endl;

	//Create ramp from dive-block finishing at partEntryPosition
	double angle = extractAngle( diveBlock );
	double divedepth = diveBlock.endPos.depth - diveBlock.startPos.depth;
	std::cout << "Detected dive-depth per cycle: " << divedepth << std::endl;
	
	double rampLength = divedepth / tan( angle );
	double tracklength = trackBlock.part.pathLength( trackBlock.startPos );
	//if the track is to short, we need to make more rounds
	if( rampLength > tracklength ){
		rampLength = tracklength;
		divedepth = tan(angle)*tracklength;
		std::cout << "Reducing dive-depth per cycle due to short track: " << divedepth << std::endl;
	}
	
	Part result;
	std::cout << "Building start block" << std::endl;
	result.append( initBlock.part );
	result.append( approachBlock.part );
	
	//Add spiral
	std::cout << "Building spiral cyle template" << std::endl;
	Part spiralCycle = buildSpiralCycle( trackBlock, angle, divedepth, cyclesBlock.startPos.depth );

	std::cout << "Building spiral" << std::endl;
	Position pos = cyclesBlock.startPos;
	//while we are too high according to machine accuracy -> add one more cycle
	while( fabs( pos.depth - cyclesBlock.endPos.depth ) > 0.00001){ 
		result.append( spiralCycle );
		pos = spiralCycle.endPosition( pos );
		
		//shift spiral cycle down
		for( size_t i=0; i<spiralCycle.size(); ++i ){
			if( spiralCycle[i].hasKey('Z') ){
				double z = std::stod( spiralCycle[i].value('Z') ) + divedepth;
				if( z< cyclesBlock.endPos.depth ) z=cyclesBlock.endPos.depth;
				spiralCycle[i].setValue( 'Z', std::to_string( z ) );
			}
		}
	}

	//Add end-ramp - add all cmds containing Z from spiralCycle but without Z
	std::cout << "Building end-ramp" << std::endl;
	for( size_t i=0; i<spiralCycle.size(); ++i ){
		if( spiralCycle[i].hasKey('Z') ){
			spiralCycle[i].remove('Z');
			result.append( spiralCycle[i] );
		} else {
			break;
		}
	}

	//Add trailing commands
	std::cout << "Building trailing block" << std::endl;
	result.append( part.subPart( cyclesBlock.endIdx ) );
	
	return result;
}

