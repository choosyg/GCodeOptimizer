#include "Part.h"

namespace gcode{

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

std::string Part::toString() const {
    std::string str;
    for( size_t i = 0; i < cmds_.size(); ++i ) {
        str += cmds_[i].toString() + "\n";
    }
    return str;
}

bool operator==( const Part& a, const Part& b ){
    if( a.size() != b.size() ){
        return false;
    }
    bool identic = true;
	for( size_t i=0; i<a.size(); ++i ){
		identic = identic && a[i] == b[i];
	}
    return identic;
}

bool operator!=( const Part& a, const Part& b ){
    return !( a== b );
}

Position endPosition( const Part& part, const Position& startPos ){
    Position pos = startPos;
    for( size_t i=0; i<part.size(); ++i ){
        pos = endPosition( part[i], pos );
    }
    return pos;
}

double length( const Part& part, const Position& startPos ){
    double l = 0.0;
    Position pos = startPos;
    for( size_t i=0; i<part.size(); ++i ){
        l += length( part[i], pos );
        pos = endPosition( part[i], pos );
    }
    return l;
}

double duration( const Part& part, const Position& startPos ){
    double fast = 8000.0;//mm/min
    double speed = 0.0;
    double duration = 0.0;
    Position pos = startPos;
    for( size_t i=0; i<part.size(); ++i ){
        if( part[i].value('G') == "00" ){
            duration += length(part[i], pos)/fast;
            pos = endPosition( part[i], pos );
        } else if( part[i].hasKey('G') ){
            if( part[i].hasKey('F') ){
                speed = std::stod( part[i].value('F') );
            }
            duration += length( part[i], pos )/speed;
            pos = endPosition( part[i], pos );
        }  else if( part[i].value('M')=="03" ){
            duration += 3.0/60.0; //Motoranlaufzeit 3 Sekunden
        }
        //Ignore other commands
    }
    return duration; //in minutes
}

}