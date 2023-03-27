#include "Part.h"

size_t Part::size() const {
    return cmds_.size();
}

bool Part::isEmpty() const{
    return cmds_.empty();
}

void Part::append( const Command& cmd ){
    cmds_.push_back( cmd );
}

void Part::append( const Part& part ){
    for( size_t i=0; i<part.size(); ++i ){
        append( part[i] );
    }
}

Part Part::subPart( size_t startIdx, size_t endIdx ) const{
    Part res;
    for( size_t i = startIdx; i<endIdx && i<cmds_.size(); ++i ){
        res.append( cmds_[i]);
    }
    return res;
}

const Command& Part::operator[]( size_t idx ) const{
    return cmds_[idx];
}

Command& Part::operator[]( size_t idx ){
    return cmds_[idx];
}

bool Part::operator==( const Part& other ) const{
    if( size() != other.size() ){
        return false;
    }
    bool identic = true;
	for( size_t i=0; i<cmds_.size(); ++i ){
		identic = identic && cmds_[i] == other[i];
	}
    return identic;
}

bool Part::operator!=( const Part& other ) const{
    return ! this->operator==( other );
}

Position Part::endPosition(  const Position& start ) const{
    Position pos = start;
    for( const auto& cmd: cmds_ ){
        pos = cmd.endPosition( pos );
    }
    return pos;
}

double Part::pathLength( const Position& startPos ) const{
    double length = 0.0;
    Position pos = startPos;
    for( const auto& cmd: cmds_ ){
        length += cmd.pathLength( pos );
        pos = cmd.endPosition( pos );
    }
    return length;
}

double Part::pathDuration( const Position& startPos ) const{
    double fast = 8000.0;//mm/min
    double speed = 0.0;
    double duration = 0.0;
    Position pos = startPos;
    for( const auto& cmd : cmds_ ){
        if( cmd.value('G') == "00" ){
            duration += cmd.pathLength(pos)/fast;
            pos = cmd.endPosition( pos );
        } else if( cmd.hasKey('G') ){
            if( cmd.hasKey('F') ){
                speed = std::stod( cmd.value('F') );
            }
            duration += cmd.pathLength( pos )/speed;
            pos =cmd.endPosition( pos );
        }
        //Ignore other commands
    }
    return duration; //in minutes
}

std::string Part::toString() const {
    std::string str;
    for( size_t i = 0; i < cmds_.size(); ++i ) {
        str += cmds_[i].toString() + "\n";
    }
    return str;
}