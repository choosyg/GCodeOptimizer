#include "Vector.h"

#include <gtest/gtest.h>

using namespace gcode;

TEST( VectorTest, ShallPointIntoCorrectDirection ) {
    Position a;
    Position b(1.0,2.0);
    Vector v( a, b );
    ASSERT_EQ( v.x, 1.0 );
    ASSERT_EQ( v.y, 2.0 );
}

TEST( VectorTest, ShallHaveCorrectScalarProduct ) {
    //Orthogonal
    Position a;
    Position b(1.0,0.0);
    Position c(0.0,1.0);
    Vector v1( a, b );
    Vector v2( a, c );
    ASSERT_EQ( v1*v2, 0.0 );

    //norm
    Position d( 5.0, 7.0 );
    Vector v3( d, c );
    ASSERT_NEAR( v3*v3, norm(v3) * norm(v3), 1e-10 );
}

TEST( VectorTest, ShallScale ) {
    Position a;
    Position b(1.0,0.0);
    Vector v1( a, b );
    ASSERT_EQ( (v1*5).x, -5.0 );
}

TEST( VectorTest, ShallHaveCorrectNorm ) {
    Position a;
    Position b(3.0,2.0);
    Vector v1( a, b );
    ASSERT_NEAR( norm(v1), 3.6055512754639891, 1e-10 );
}

TEST( VectorTest, ShallHaveCorrectDirection ) {
    Position a;
    Position b(1.0,0.0);
    Position c(0.0,1.0);
    Vector v1( a, b );
    Vector v2( a, c );

    ASSERT_EQ( rotationDirection( v1, v2 ), Direction::CounterClockwise );
    ASSERT_EQ( rotationDirection( v2, v1 ), Direction::Clockwise );
}