#include "Command.h"

#include "Vector.h"

#include <algorithm>
#include <cmath>

namespace{
    std::vector< std::string > split( const std::string& str, char d ){
        size_t start = 0;
        size_t end = str.find( d );
        std::vector< std::string > res;

        while( end != std::string::npos ) {
            res.push_back( str.substr( start, end-start ) );
            start = end + 1;
            end = str.find( d, start );
        }
        res.push_back( str.substr( start ) );

        return res;
    }
}

Command::Command( const std::string& str )
{
    if( str[0] == '(' ){
        comment_ = str;
        return;
    }
    
    auto params = split( str, ' ' );
    for( const auto& p : params ){
        setValue( p[0], p.substr(1) );
    }
}

bool Command::isComment() const{
    return !comment_.empty();
}

bool Command::hasKey( char c ) const{
    auto it = std::find_if( params_.begin(), params_.end(), [&](const Param& p){return p.key==c;});
    return it != params_.end();
}

const std::string& Command::value(char c) const{
    auto it = std::find_if( params_.begin(), params_.end(), [&](const Param& p){return p.key==c;});
    if( it == params_.end() ){
        return comment_;
    } else {
        return it->value;
    } 
}

void Command::setValue( char c, const std::string& value ){
    auto it = std::find_if( params_.begin(), params_.end(), [&](const Param& p){return p.key==c;});
    if( it == params_.end() ){
        params_.emplace_back( c, value );
    } else {
        it->value = value;
    }
}

void Command::remove( char c ){
    auto it = std::find_if( params_.begin(), params_.end(), [&](const Param& p){return p.key==c;});
    if( it != params_.end() ){
        params_.erase( it );
    }
}

Position Command::endPosition( const Position& start ) const{
    Position p = start;
    if( hasKey('X') ) p.x = std::stod( value('X') );
    if( hasKey('Y') ) p.y = std::stod( value('Y') );
    if( hasKey('Z') ) p.depth = std::stod( value('Z') );
    return p;
}

double Command::pathLength( const Position& start ) const{
    if( isComment() || params_.front().key != 'G' ){
        return 0.0;
    }

    auto endPos = endPosition( start );
    auto code = std::stoul( params_.front().value );
    if( code <= 1 ){
        //(Fast) liear move
        return dist( start, endPos );
    } else if ( code <= 3 ){
        //The center of the circle (spiral)
        auto i = std::stod( value('I') );
        auto j = std::stod( value('J') );
        Position center( start.x+i, start.y+j );

        //the angle can be calculated from the scalar product
        Vector ca( center, start );
        Vector cb( center, endPos );
        double angle = acos( (ca*cb)/ca.norm()/cb.norm() );

        //The direction of the angle can be calculated by the cross product
        double zd = ca.cross(cb);
        if( code == 2 && zd>0.0 || code == 3 && zd<0.0){
            angle = 2*M_PI-angle;
        }

        return ca.norm()*angle;
    }
    return 0.0;
}

std::string Command::toString() const{
    std::string str = comment_;

    if( !params_.empty() ){
        //Add params, maintain default order
        auto copy = params_;
        std::vector<char> order = { 'G', 'M', 'X', 'Y', 'Z', 'I', 'J', 'F', 'S' };
        for( const auto& c: order ){
            auto it = std::find_if( copy.begin(), copy.end(), [&](const Command::Param& p){return p.key==c;});
            if( it != copy.end() ){
                str += it->key + it->value + " ";
                copy.erase( it );
            }
        }
        //Add unknown params
        for( const auto & p: copy ){
            str += p.key + p.value + " ";
        }
        //remove trailing " "
        str.erase( str.size()-1 );
    }
    return str;
}

bool Command::operator==( const Command& other ) const{
    if( params_.size() != other.params_.size() ){
		return false;
	}
	bool identic = true;
	for( size_t i=0; i<params_.size(); ++i ){
		identic = identic && params_[i] == other.params_[i];
	}
    return identic;
}

bool Command::operator!=( const Command& other ) const{
    return !this->operator==( other );
}