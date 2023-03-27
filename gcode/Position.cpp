#include "Position.h"

#include <cmath>

Position::Position(double xx, double yy, double d) 
: x(xx),y(yy),depth(d){
}

bool Position::operator== ( const Position& other ) const{
    return x == other.x && y == other.y;
}

bool Position::operator!= ( const Position& other ) const{
    return !this->operator==(other);
}

double dist( const Position& a, const Position& b ){
    double dx = b.x-a.x;
    double dy = b.y-a.y;
    return sqrt( dx*dx + dy*dy );
}