#include "Part.h"

#include <gtest/gtest.h>

#include <cmath>

using namespace gcode;

TEST( PartTest, ShouldHaveCorrectPathLength ) {
    Part part;
    part.append( Command( "G00 X1 Y0 Z1" ) );
    part.append( Command( "G01 X1 Y2 Z2" ) );

    ASSERT_EQ( 3.0, length( part, Position( 0.0, 0.0, 0.0 ) ) );
}

TEST( PartTest, ShouldHaveCorrectPathLengthCircle ) {
    Part part;
    part.append( Command( "G02 X0 Y1 Z1 I1 J0" ) );
    part.append( Command( "G02 X1 Y0 Z1 I0 J-1" ) );
    part.append( Command( "G03 X0 Y1 Z1 I-1 J0" ) );
    part.append( Command( "G03 X-1 Y0 Z1 I0 J-1" ) );

    ASSERT_NEAR( 2*M_PI, length( part, Position( -1.0, 0.0, 0.0 ) ), 0.001 );
}

TEST( PartTest, ShouldHaveCorrectPathDurationFast ) {
    Part part;
    part.append( Command( "G00 X8000 Y0 Z1" ) );
    ASSERT_EQ( 1.0, duration( part, Position( 0.0, 0.0, 0.0 ) ) );
}

TEST( PartTest, ShouldHaveCorrectPathDurationSpecified ) {
    Part part;
    part.append( Command( "G01 X8000 Y0 Z1 F2000" ) );
    ASSERT_EQ( 4.0, duration( part, Position( 0.0, 0.0, 0.0 ) ) );
}

TEST( PartTest, ShouldHaveCorrectPathDurationSpecifiedCircle ) {
    Part part;
    part.append( Command( "G02 X1000 Y0 Z1 I0 J-1000 F1000" ) );
    ASSERT_EQ( M_PI/2.0, duration( part, Position( 0.0, 1000.0, 0.0 ) ) );
}