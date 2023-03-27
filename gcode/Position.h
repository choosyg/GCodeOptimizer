#pragma once

class Position{
    public:
		Position(double xx, double yy, double zz);
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        bool operator== ( const Position& other ) const;
        bool operator!= ( const Position& other ) const;
};