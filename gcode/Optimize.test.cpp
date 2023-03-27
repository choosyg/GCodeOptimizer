#include "Optimize.h"

#include <gtest/gtest.h>

TEST( GCodeTest, ShouldUpdatePartPositions ) {
    Part part;
    part.append( Command( "G01 X1 Y1 Z1", Position( 0.0, 0.0, 0.0 ) ) );
    ASSERT_EQ( part.before(), Position( 0.0, 0.0, 0.0 ) );
    ASSERT_EQ( part.after(), Position( 1.0, 1.0, 1.0 ) );

    part.append( Command( "G01 X0 Y0 Z1.1", part.after() ) );
    ASSERT_EQ( part.before(), Position( 0.0, 0.0, 0.0 ) );
    ASSERT_EQ( part.after(), Position( 0.0, 0.0, 1.1 ) );
}

TEST( GCodeTest, ShouldIgnoreZOnPositionCompare ) {
    Part part;
    part.append( Command( "G01 X1 Y1 Z1", Position( 0.0, 0.0, 0.0 ) ) );
    ASSERT_EQ( part.before(), Position( 0.0, 0.0, 123.456 ) );
}

TEST( GCodeTest, ShouldOptimizeMinimum ) {
    Part part;
    part.append( Command( "G01 X1.0 Y1.0 Z-1.0", Position( 0.0, 0.0, 0.0 ) ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-1.1", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0", part.after() ) );
    part.append( Command( "G01 X0.0 Y0.0", part.after() ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-2.0", part.after() ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-2.2", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0", part.after() ) );

    auto res = optimizePart( part );

    ASSERT_EQ( res.size(), 5 ); //Note that the final Block was not optimized
    ASSERT_EQ( res[0].toString(), "G01 X2.0 Y2.0 Z-1.100000" ); // Z should not become -2 here!
    ASSERT_EQ( res[1].toString(), "G01 X0.0 Y0.0" );
}

TEST( GCodeTest, ShouldOptimizeBigerBlock ) {
    Part part;
    part.append( Command( "G01 X1.0 Y1.0 Z-1.0", Position( 0.0, 0.0, 0.0 ) ) );
    part.append( Command( "G01 X2.0 Y2.0 Z-1.1", part.after() ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-1.2", part.after() ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-2.0", part.after() ) );
    part.append( Command( "G01 X1.0 Y1.0", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0", part.after() ) );
    part.append( Command( "G01 X0.0 Y0.0", part.after() ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-3.0", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0 Z-3.1", part.after() ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-3.2", part.after() ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-4.0", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0", part.after() ) );

    auto res = optimizePart( part );

    ASSERT_EQ( res.size(), 8 ); //Note that the final Block was not optimized
    ASSERT_EQ( res[0].toString(), "G01 X1.0 Y1.0 Z-2.000000" ); 
    ASSERT_EQ( res[1].toString(), "G01 X2.0 Y2.0" ); //Note that the second comand has no Z added since -2 was reached before
}

TEST( GCodeTest, ShouldNotOptimizeSingleBlock ) {
    Part part;
    part.append( Command( "G01 X1 Y1 Z1", Position( 0.0, 0.0, 0.0 ) ) );
    part.append( Command( "G01 X0 Y0 Z1.1", part.after() ) );
    part.append( Command( "G01 X2 Y2", part.after() ) );
    
    auto res = optimizePart( part );

    ASSERT_EQ( res.toString(), part.toString() );
}

TEST( GCodeTest, ShouldNotOptimizeIfZBlockInBetween ) {
    Part part;
    part.append( Command( "G01 X1.0 Y1.0 Z-1.0", Position( 0.0, 0.0, 0.0 ) ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-1.1", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0 Z1.0", part.after() ) );//this z position change should block optimization
    part.append( Command( "G01 X0.0 Y0.0", part.after() ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-2.0", part.after() ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-2.2", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0", part.after() ) );

    auto res = optimizePart( part );

    ASSERT_EQ( res.toString(), part.toString() );
}

TEST( GCodeTest, ShouldNotOptimizeIfNextZBlockIsColinearButAtWrongPosition ) {
    Part part;
    part.append( Command( "G01 X1.0 Y1.0 Z-1.0", Position( 0.0, 0.0, 0.0 ) ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-1.1", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0", part.after() ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-2.0", part.after() ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-2.2", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0", part.after() ) );

    auto res = optimizePart( part );

    ASSERT_EQ( res.toString(), part.toString() );
}

TEST( GCodeTest, ShouldNotOptimizeIfFollowingCommandsDoNotMatch ) {
    Part part;
    part.append( Command( "G01 X1.0 Y1.0 Z-1.0", Position( 0.0, 0.0, 0.0 ) ) );
    part.append( Command( "G01 X2.0 Y2.0 Z-1.1", part.after() ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-1.2", part.after() ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-2.0", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0", part.after() ) ); // This command should travel to 1/1
    part.append( Command( "G01 X2.0 Y2.0", part.after() ) );
    part.append( Command( "G01 X0.0 Y0.0", part.after() ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-3.0", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0 Z-3.1", part.after() ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-3.2", part.after() ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-4.0", part.after() ) );
    part.append( Command( "G01 X2.0 Y2.0", part.after() ) );

    auto res = optimizePart( part );

    ASSERT_EQ( res.toString(), part.toString() );
}

