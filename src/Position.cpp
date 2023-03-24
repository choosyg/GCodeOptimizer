#include "Position.h"

Position::Position(double xx, double yy, double zz) 
: x(xx),y(yy),z(zz){
}

bool Position::operator== ( const Position& other ) const{
    //z is ignored!
    return x == other.x && y == other.y;
}

bool Position::operator!= ( const Position& other ) const{
    //z is ignored!
    return !this->operator==(other);
}