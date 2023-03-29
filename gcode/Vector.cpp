#include "Vector.h"

#include <cmath>

namespace gcode{

Vector::Vector( const Position& a, const Position& b ) :x( b.x-a.x ), y( b.y-a.y ){}

double operator*( const Vector& a, const Vector& b ){
    return a.x*b.x + a.y*b.y;
}

Vector operator*( const Vector& a, double v ){
    return Vector( a.x*v, a.y*v );
}

Vector operator*( double v, const Vector& a ){
    return a*v;
}

double norm( const Vector& v){
    return sqrt( v*v );
}

Direction rotationDirection( const Vector& a, const Vector& b ){
    return a.x*b.y-a.y*b.x < 0 ? Direction::Clockwise : Direction::CounterClockwise;
}

}