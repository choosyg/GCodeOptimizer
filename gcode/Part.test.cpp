/*#include "Part.h"

#include <gtest/gtest.h>

TEST( PartTest, ShouldUpdatePartPositions ) {
    Part part;
    part.append( Command( "G01 X1 Y1 Z1", Position( 0.0, 0.0, 0.0 ) ) );
    ASSERT_EQ( part.before(), Position( 0.0, 0.0, 0.0 ) );
    ASSERT_EQ( part.after(), Position( 1.0, 1.0, 1.0 ) );

    part.append( Command( "G01 X0 Y0 Z1.1", part.after() ) );
    ASSERT_EQ( part.before(), Position( 0.0, 0.0, 0.0 ) );
    ASSERT_EQ( part.after(), Position( 0.0, 0.0, 1.1 ) );
}

TEST( PartTest, ShouldIgnoreZOnPositionCompare ) {
    Part part;
    part.append( Command( "G01 X1 Y1 Z1", Position( 0.0, 0.0, 0.0 ) ) );
    ASSERT_EQ( part.before(), Position( 0.0, 0.0, 123.456 ) );
}*/