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

using namespace gcode;

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
		res.endPos = endPosition( res.part, start );
		return res;
	}

	Block extractApproachBlock( const Part& part, size_t idx, const Position& start ){
		Block res( idx, start );
		while( idx<part.size() && ( part[idx].value('G') == "00" || (!part[idx].hasKey('X') && !part[idx].hasKey('Y')) ) ){ 
			res.part.append( part[idx] );
			++idx; 
		}
		res.endIdx = idx;
		res.endPos = endPosition( res.part, start );
		return res;
	}

	Block extractDiveBlock( const Part& part, size_t idx, const Position& start ){
		Block res( idx, start );
		while( idx<part.size() && part[idx].hasKey('Z') ){
			res.part.append( part[idx] );
			++idx;
		}
		res.endIdx = idx;
		res.endPos = endPosition( res.part, start );
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
		res.endPos = endPosition( res.part, start );
		std::cout << "Detected track-block length: " << res.part.size() << std::endl;
		return res;
	}

	Block extracktCyclesBlock( const Part& part, const Part& cycle, size_t idx, const Position& start ){
		Block res( idx, start );
		while(idx<part.size()){
			Part candidate = removeZ( part.subPart( idx, idx+cycle.size() ) );
			if( candidate == cycle ){
				res.part.append( part.subPart( idx, idx+cycle.size() ) );
				idx += cycle.size();
			} else { 
				break; 
			}
		}
		res.endIdx = idx;
		res.endPos = endPosition( res.part, start );
		return res;
	}

	double extractDiveAngle( const Block& diveBlock ){
		auto pos = endPosition( diveBlock.part[0], diveBlock.startPos );
		double zdiff= pos.depth - diveBlock.startPos.depth;
		double l = length( diveBlock.part[0], diveBlock.startPos );
		double angle = atan( zdiff / l );
		std::cout << "Detected dive-angle: " << angle/2.0/M_PI*360.0 << "°" << std::endl;
		return angle;
	}

	bool validate( const Block& dive, const Block& track ){
		size_t steps = dive.part.size()/2;
		if( track.part.size() < steps ){
			return false;
		}

		bool valid = true;
		for( size_t i=0; i<steps-1 && valid; ++i ){ // the last cmd may differ
			auto dc = dive.part[i];
			dc.remove('Z');
			dc.remove('F');
			auto tc = track.part[i];
			tc.remove('F');
			valid = valid && dc == tc;
		}
		return valid;
	}

	Part buildSpiralCycle( const Block& track, double angle, double cycledepth, double startz, const std::string& divespeed ){
		Position pos = track.startPos;
		pos.depth = startz;

		//build dive part and track part
		Part cycle;
		size_t idx = 0;
		double depth = 0;
		while( depth > cycledepth ){
			auto cmd = track.part[idx];
			double l = length( cmd, pos );
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
				pos = endPosition( cmd, pos );
				++idx;
			}
		}
		cycle.append( track.part.subPart( idx ) );

		//Fix speeds
		if( !divespeed.empty() ){
			cycle[0].setValue( 'F', divespeed );
			auto trackspeed = track.part[0].value('F');
			for( size_t i=0; i<cycle.size(); ++i ){
				if( !cycle[i].hasKey('Z') ){
					cycle[i].setValue( 'F', trackspeed );
					break;
				}
			}
		}

		return cycle;
	}
}

namespace gcode{

Part optimizePart(const Part& part, const Position& start ) {
	if( part.isEmpty() ){
		std::cout << "Skipping empty part" << std::endl;
		return part;
	}
	std::cout << "***********************************************************************************************" << std::endl;
	std::cout << "* Analyzing part " << part[0].toString() << std::endl; //typically a comment with the name of the part
	std::cout << "***********************************************************************************************" << std::endl;

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

	if( trackBlock.endIdx == part.size() ){
		std::cout << "Discarded - part has single track" << std::endl;
		return part;
	}

	if( !validate( diveBlock, trackBlock ) ){
		std::cout << "Discarded - track-block does not match dive-block" << std::endl;
		return part;
	}

	//dive + track are the complete cycle that will repeat now -> build a cycle template from both blocks
	Part cycleTemplate = removeZ( diveBlock.part );
	cycleTemplate.append( trackBlock.part );
	//diveblock and track block might have different "F" values. In that case the first line of track has "F" 
	//but the first line of dive has an "F" only in the following cycles -> fix that
	std::string diveSpeed;
	if( trackBlock.part[0].hasKey('F') && !diveBlock.part[0].hasKey('F') ){
		diveSpeed = part[trackBlock.endIdx].value('F');
		if( diveSpeed.empty() ){
			std::cout << "Discarded - unable to find correct dive-speed" << std::endl;
			return part;
		}
		std::cout << "Detected dive-speed: " << diveSpeed << std::endl;
		std::cout << "Detected track-speed: " << trackBlock.part[0].value('F') << std::endl;
		cycleTemplate[0].setValue('F',diveSpeed);
	}
	
    // build a Block containing all cycles excluding dive and track found above
	auto cyclesBlock = extracktCyclesBlock( part, cycleTemplate, trackBlock.endIdx, trackBlock.endPos );
	if( cyclesBlock.part.size()/cycleTemplate.size() < 1 ){
		std::cout << "Discarded - not enough cycles for optimization" << std::endl;
		return part;
	}
	
	//Create ramp from dive-block finishing at partEntryPosition
	double angle = extractDiveAngle( diveBlock );
	double divedepth = diveBlock.endPos.depth - diveBlock.startPos.depth;
	std::cout << "Detected dive-depth per cycle: " << divedepth << std::endl;
	
	double rampLength = divedepth / tan( angle );
	double tracklength = length( trackBlock.part, trackBlock.startPos );
	//if the track is to short, we need to make more rounds
	if( rampLength > tracklength ){
		rampLength = tracklength;
		divedepth = tan(angle)*tracklength;
		std::cout << "Reducing dive-depth per cycle due to short track: " << divedepth << std::endl;
	}
	
	Part result;
	std::cout << "Building Optimized GCode" << std::endl;
	result.append( initBlock.part );
	result.append( approachBlock.part );
	
	//Add spiral
	Part spiralCycle = buildSpiralCycle( trackBlock, angle, divedepth, diveBlock.startPos.depth, diveSpeed );

	Position pos = diveBlock.startPos;
	//while we are too high according to machine accuracy -> add one more cycle
	while( fabs( pos.depth - cyclesBlock.endPos.depth ) > 0.00001){ 
		result.append( spiralCycle );
		pos = endPosition( spiralCycle, pos );
		
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
	std::vector<Command> invers;
	for( size_t i=0; i<spiralCycle.size(); ++i ){
		if( spiralCycle[i].hasKey('Z') ){
			spiralCycle[i].remove('Z');
			spiralCycle[i].remove('F');
			result.append( spiralCycle[i] );
			invers.push_back( invert( spiralCycle[i], pos ) );
			pos = endPosition( spiralCycle[i], pos );
		} else {
			break;
		}
	}
	for( size_t i=invers.size(); i>0; --i ){
		result.append( invers[i-1] );
	}

	//Add trailing commands
	result.append( part.subPart( cyclesBlock.endIdx ) );
	
	return result;
}

}