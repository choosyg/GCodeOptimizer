#pragma once

#include "Position.h"

namespace gcode{

class Vector{
    public:
        Vector( const Position& a, const Position& b );
        double x;
        double y;
};

double operator*( const Vector& a, const Vector& b );    
Vector operator*( const Vector& a, double v );
Vector operator*( double v, const Vector& a );

double norm( const Vector& v );

enum class Direction { Clockwise, CounterClockwise };
Direction rotationDirection( const Vector& a, const Vector& b );

}