#pragma once

#include "Position.h"

class Vector{
    public:
        double x;
        double y;

        Vector( const Position& a, const Position& b );
        
        double operator*( const Vector& other ) const;    
        Vector operator*( double v ) const;

        double norm() const;

        //z-component of the cross product to have the orientation to the other vector
        double cross( const Vector& other ) const;
};