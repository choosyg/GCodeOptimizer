#include "Command.h"

#include "Vector.h"

#include <algorithm>
#include <cmath>

namespace gcode{
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

bool operator==( const Command& a, const Command& b ){
    return a.toString() == b.toString();
}

bool operator!=(  const Command& a, const Command& b  ){
    return !(a == b );
}

Position endPosition( const Command& cmd, const Position& start ){
    Position p = start;
    if( cmd.hasKey('X') ) p.x = std::stod( cmd.value('X') );
    if( cmd.hasKey('Y') ) p.y = std::stod( cmd.value('Y') );
    if( cmd.hasKey('Z') ) p.depth = std::stod( cmd.value('Z') );
    return p;
}

double length( const Command& cmd, const Position& start ){
    if( cmd.isComment() || !cmd.hasKey('G') ){
        return 0.0;
    }

    auto endPos = endPosition( cmd, start );
    auto code = std::stoul( cmd.value('G') );
    if( code <= 1 ){
        //(Fast) liear move
        return dist( start, endPos );
    } else if ( code <= 3 ){
        //The center of the circle (spiral)
        auto i = std::stod( cmd.value('I') );
        auto j = std::stod( cmd.value('J') );
        Position center( start.x+i, start.y+j );

        //the angle can be calculated from the scalar product
        Vector ca( center, start );
        Vector cb( center, endPos );
        double angle = acos( (ca*cb)/norm(ca)/norm(cb) );

        //The direction of the angle can be calculated by the cross product
        auto zd = rotationDirection( ca, cb );
        if( code == 2 && zd==Direction::CounterClockwise || code == 3 && zd==Direction::Clockwise){
            angle = 2*M_PI-angle;
        }

        return norm(ca)*angle;
    }
    return 0.0;
}

std::array< Command, 2 > split( const Command& cmd, const Position& pos, double percent ){
    auto endPos = endPosition( cmd, pos );
		
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
        double angle = acos( (ca*cb)/norm(ca)/norm(cb) );

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

}