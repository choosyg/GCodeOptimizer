#pragma once

#include <string>

namespace gcode{

class Position{
    public:
		Position(double xx = 0.0, double yy = 0.0, double depth = 0.0);
        double x = 0.0;
        double y = 0.0;
        double depth = 0.0;

        std::string toString() const;
};

bool operator== ( const Position& a, const Position& b ); // compares x/y
bool operator!= ( const Position& a, const Position& b ); // compares x/y

double dist( const Position& a, const Position& b );

}