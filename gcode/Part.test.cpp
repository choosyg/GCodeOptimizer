#include "Part.h"

#include <gtest/gtest.h>

#include <cmath>

TEST( PartTest, ShouldHaveCorrectPathLength ) {
    Part part;
    part.append( Command( "G00 X1 Y0 Z1" ) );
    part.append( Command( "G01 X1 Y2 Z2" ) );

    ASSERT_EQ( 3.0, part.pathLength( Position( 0.0, 0.0, 0.0 ) ) );
}

TEST( PartTest, ShouldHaveCorrectPathLengthCircle ) {
    Part part;
    part.append( Command( "G02 X0 Y1 Z1 I0 J1" ) );
    part.append( Command( "G02 X1 Y0 Z1 I1 J0" ) );
    part.append( Command( "G03 X0 Y1 Z1 I0 J1" ) );
    part.append( Command( "G03 X-1 Y0 Z1 I-1 J0" ) );

    ASSERT_NEAR( 2*M_PI, part.pathLength( Position( -1.0, 0.0, 0.0 ) ), 0.001 );
}

TEST( PartTest, ShouldHaveCorrectPathDurationFast ) {
    Part part;
    part.append( Command( "G00 X8000 Y0 Z1" ) );
    ASSERT_EQ( 1.0, part.pathDuration( Position( 0.0, 0.0, 0.0 ) ) );
}

TEST( PartTest, ShouldHaveCorrectPathDurationSpecified ) {
    Part part;
    part.append( Command( "G01 X8000 Y0 Z1 F2000" ) );
    ASSERT_EQ( 4.0, part.pathDuration( Position( 0.0, 0.0, 0.0 ) ) );
}

TEST( PartTest, ShouldHaveCorrectPathDurationSpecifiedCircle ) {
    Part part;
    part.append( Command( "G02 X1000 Y0 Z1 I1000 J0 F1000" ) );
    ASSERT_EQ( M_PI/2.0, part.pathDuration( Position( 0.0, 1000.0, 0.0 ) ) );
}