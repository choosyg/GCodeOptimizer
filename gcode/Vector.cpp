#include "Vector.h"

#include <cmath>

Vector::Vector( const Position& a, const Position& b ) :x( b.x-a.x ), y( b.y-a.y ){}
        
double Vector::operator*( const Vector& other ) const {
    return x*other.x + y*other.y;
}

Vector Vector::operator*( double v ) const{
    return Vector( x*v, y*v );
}

double Vector::norm() const{
    return sqrt( x*x + y*y );
}

//z-component of the cross product to have the orientation to the other vector
double Vector::cross( const Vector& other ) const {
    return x*other.y-y*other.x;
}