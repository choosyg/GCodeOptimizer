#include "Position.h"

#include <gtest/gtest.h>

#include <cmath>

TEST( PositionTest, ShouldCompareCorrect ) {
    Position pos1( 1.0, 2.0);
    Position pos2( 1.0, 2.0, 2.0);
    Position pos3( 1.0, 3.0);

    ASSERT_EQ( pos1, pos2 );
    ASSERT_NE( pos1, pos3 );
}

TEST( PositionTest, ShouldCalculateDistance ) {
    Position pos1( 1.0, 0.0);
    Position pos2( 0.0, 1.0);

    ASSERT_EQ( sqrt(2.0), dist( pos1, pos2 ) );
}