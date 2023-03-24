#include "Position.h"

bool Position::operator== ( const Position& other ) const{
    //z is ignored!
    return x == other.x && y == other.y;
}

bool Position::operator!= ( const Position& other ) const{
    //z is ignored!
    return !this->operator==(other);
}