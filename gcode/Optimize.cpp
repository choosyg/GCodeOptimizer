#include "Optimize.h"

#include "Command.h"
#include "Position.h"
#include "Part.h"

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

	Part cycleTemplate = removeZ( diveBlock.part );
	cycleTemplate.append( trackBlock.part );
	auto cyclesBlock = extracktCyclesBlock( part, cycleTemplate, diveBlock.startIdx, diveBlock.startPos );
	if( cyclesBlock.part.size()/cycleTemplate.size() < 2 ){
		std::cout << "Discarded - not enough cycles for optimization" << std::endl;
		return part;
	}
	
	std::cout << "Optimizing..." << std::endl;

	//Create ramp from dive-block finishing at partEntryPosition
	double angle = extractAngle( diveBlock );
	double divedepth = diveBlock.endPos.depth - diveBlock.startPos.depth;
	std::cout << "Detected dive-depth: " << divedepth << std::endl;
	
	double rampLength = divedepth / tan( angle );
	double tracklength = trackBlock.part.pathLength( trackBlock.startPos );
	//if the track is to short, we need to make more rounds
	if( rampLength > tracklength ){
		rampLength = tracklength;
		divedepth = tan(angle)*tracklength;
		std::cout << "Reducing dive-depth due to short track: " << divedepth << std::endl;
	}
	
	Part result;
	std::cout << "Building start block" << std::endl;
	result.append( initBlock.part );
	result.append( approachBlock.part );
	
	//Add spiral
	std::cout << "Building spiral" << std::endl;
	Position spiralPosition = cyclesBlock.startPos;
	size_t trackidx = 0;
	while( spiralPosition.depth != cyclesBlock.endPos.depth ){
		//build ramp
		double rampdepth = 0.0;
		while( rampdepth > divedepth ){
			auto cmd = trackBlock.part[trackidx];
			double l = cmd.pathLength( spiralPosition );
			double depthDelta = tan(angle)*l;
			rampdepth += depthDelta;
			//Assert depth per round is not increased
			if( rampdepth < divedepth ){
				depthDelta = divedepth - rampdepth + depthDelta;
				rampdepth = divedepth;
			}
			//Assert maximal depth is not increased
			if( spiralPosition.depth+depthDelta < cyclesBlock.endPos.depth ){
				cmd.setValue('Z', std::to_string( cyclesBlock.endPos.depth ) );
			} else {
				cmd.setValue('Z', std::to_string( spiralPosition.depth+depthDelta ) );
			}
			spiralPosition = cmd.endPosition( spiralPosition );
			result.append( cmd );
			++trackidx;
		}
		//add rest of track
		auto tr = trackBlock.part.subPart(trackidx);
		result.append( tr );
		spiralPosition = tr.endPosition( spiralPosition );
		std::cout << "Built cycle " << spiralPosition.depth << std::endl;
		trackidx = 0;
	}
	//Add end-ramp
	std::cout << "Building end-ramp" << std::endl;
	//TODO

	//Add trailing commands
	std::cout << "Building trailing block" << std::endl;
	result.append( part.subPart( cyclesBlock.endIdx ) );
	
	return result;
}

