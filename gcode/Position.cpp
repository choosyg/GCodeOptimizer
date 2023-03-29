#include "Position.h"

#include <cmath>

namespace gcode{

Position::Position(double xx, double yy, double d) 
: x(xx),y(yy),depth(d){
}

std::string Position::toString() const{
    return "("+std::to_string(x) + ", " + std::to_string(y)+")";
}

bool operator== ( const Position& a, const Position& b ){
    return a.x == b.x && a.y == b.y;
}

bool operator!= ( const Position& a, const Position& b ){
    return !(a==b);
}

double dist( const Position& a, const Position& b ){
    double dx = b.x-a.x;
    double dy = b.y-a.y;
    return sqrt( dx*dx + dy*dy );
}

}