#pragma once

class Position{
    public:
		Position(double xx = 0.0, double yy = 0.0, double depth = 0.0);
        double x = 0.0;
        double y = 0.0;
        
        double depth = 0.0;

        bool operator== ( const Position& other ) const; // compares x/y
        bool operator!= ( const Position& other ) const; // compares x/y
};

double dist( const Position& a, const Position& b );