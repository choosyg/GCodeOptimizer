#include "Optimize.h"

#include <gtest/gtest.h>

TEST( OptimizeTest, ShouldOptimizeMinimumLinear ) {
    Part part;
    part.append( Command( "G01 X1.0 Y1.0 Z-1.0" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-1.1" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );
    part.append( Command( "G01 X0.0 Y0.0" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-2.0" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-2.2" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );
    part.append( Command( "G01 X0.0 Y0.0" ) );

    auto res = optimizePart( part, Position() );

    ASSERT_EQ( res.size(), 4 ); //Note that the final Block was not optimized
    ASSERT_EQ( res[0].toString(), "G01 X2.0 Y2.0 Z-1.100000" ); // Z should not become -2 here!
    ASSERT_EQ( res[1].toString(), "G01 X0.0 Y0.0" );
}

TEST( OptimizeTest, ShouldOptimizeMinimumCircular ) {
    Part part;
    part.append( Command( "G02 X1.0 Y0.0 I1.0 J0.0 Z-1.0" ) );
    part.append( Command( "G03 X0.0 Y1.0 I0.0 J1.0 Z-1.1" ) );
    part.append( Command( "G02 X0.0 Y-1.0 I0.0 J-1.0" ) );
    part.append( Command( "G01 X0.0 Y1.0" ) );
    part.append( Command( "G02 X1.0 Y0.0 I1.0 J0.0 Z-2.0" ) );
    part.append( Command( "G03 X0.0 Y1.0 I0.0 J1.0 Z-2.2" ) );
    part.append( Command( "G02 X0.0 Y-1.0 I0.0 J-1.0" ) );
    part.append( Command( "G01 X0.0 Y1.0" ) );

    auto res = optimizePart( part, Position(0.0,1.0) );

    EXPECT_EQ( res.size(), 4 ); //Note that the final Block was not optimized
    EXPECT_EQ( res[0].toString(), "G02 X0.0 Y-1.0 Z-1.100000 I0.0 J-1.0" );
    EXPECT_EQ( res[1].toString(), "G01 X0.0 Y1.0" );
}

TEST( OptimizeTest, ShouldOptimizeMinimumCircular2 ) {
    Part part;
    part.append( Command( "G02 X1.0 Y0.0 I1.0 J0.0 Z-1.0" ) );
    part.append( Command( "G03 X0.0 Y1.0 I0.0 J1.0 Z-2.0" ) );
    part.append( Command( "G02 X1.0 Y0.0 I1.0 J0.0" ) );
    part.append( Command( "G01 X1.0 Y1.0" ) );
    part.append( Command( "G01 X0.0 Y1.0" ) );
    part.append( Command( "G02 X1.0 Y0.0 I1.0 J0.0 Z-3.0" ) );
    part.append( Command( "G03 X0.0 Y1.0 I0.0 J1.0 Z-4.0" ) );
    part.append( Command( "G02 X1.0 Y0.0 I1.0 J0.0" ) );
    part.append( Command( "G01 X1.0 Y1.0" ) );
    part.append( Command( "G01 X0.0 Y1.0" ) );

    auto res = optimizePart( part, Position(0.0,1.0) );

    EXPECT_EQ( res.size(), 6 ); //Note that the final Block was not optimized
    EXPECT_EQ( res[0].toString(), "G02 X1.0 Y0.0 Z-1.000000 I1.0 J0.0" );
    EXPECT_EQ( res[1].toString(), "G01 X1.0 Y1.0 Z-1.636620" );
    EXPECT_EQ( res[2].toString(), "G01 X0.0 Y1.0 Z-2.000000" );
}

TEST( OptimizeTest, ShouldOptimizeBigerBlock ) {
    Part part;
    part.append( Command( "G01 X1.0 Y1.0 Z-1.0" ) );
    part.append( Command( "G01 X2.0 Y2.0 Z-1.1" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-1.2" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-2.0" ) );
    part.append( Command( "G01 X1.0 Y1.0" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );
    part.append( Command( "G01 X0.0 Y0.0" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-3.0" ) );
    part.append( Command( "G01 X2.0 Y2.0 Z-3.1" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-3.2" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-4.0" ) );
    part.append( Command( "G01 X1.0 Y1.0" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );
    part.append( Command( "G01 X0.0 Y0.0" ) );

    auto res = optimizePart( part, Position() );

    EXPECT_EQ( res.size(), 6 ); //Note that the final Block was not optimized
    EXPECT_EQ( res[0].toString(), "G01 X1.0 Y1.0 Z-1.000000" ); 
    EXPECT_EQ( res[1].toString(), "G01 X2.0 Y2.0 Z-2.000000" ); //Note that the second comand has no Z added since -2 was reached before
}

TEST( OptimizeTest, ShouldNotOptimizeSingleBlock ) {
    Part part;
    part.append( Command( "G01 X1 Y1 Z1" ) );
    part.append( Command( "G01 X0 Y0 Z1.1" ) );
    part.append( Command( "G01 X2 Y2" ) );
    
    auto res = optimizePart( part, Position() );

    ASSERT_EQ( res.toString(), part.toString() );
}

TEST( OptimizeTest, ShouldNotOptimizeIfZBlockInBetween ) {
    Part part;
    part.append( Command( "G01 X1.0 Y1.0 Z-1.0" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-1.1" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );
    part.append( Command( "G01 X2.0 Y2.0 Z1.0" ) );//this z position change should block optimization
    part.append( Command( "G01 X0.0 Y0.0" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-2.0" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-2.2" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );

    auto res = optimizePart( part, Position() );

    ASSERT_EQ( res.toString(), part.toString() );
}

TEST( OptimizeTest, ShouldNotOptimizeIfNextZBlockIsColinearButAtWrongPosition ) {
    Part part;
    part.append( Command( "G01 X1.0 Y1.0 Z-1.0" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-1.1" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-2.0" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-2.2" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );

    auto res = optimizePart( part, Position() );

    ASSERT_EQ( res.toString(), part.toString() );
}

TEST( OptimizeTest, ShouldNotOptimizeIfFollowingCommandsDoNotMatch ) {
    Part part;
    part.append( Command( "G01 X1.0 Y1.0 Z-1.0" ) );
    part.append( Command( "G01 X2.0 Y2.0 Z-1.1" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-1.2" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-2.0" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) ); // This command should travel to 1/1
    part.append( Command( "G01 X2.0 Y2.0" ) );
    part.append( Command( "G01 X0.0 Y0.0" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-3.0" ) );
    part.append( Command( "G01 X2.0 Y2.0 Z-3.1" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-3.2" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-4.0" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );

    auto res = optimizePart( part, Position() );

    ASSERT_EQ( res.toString(), part.toString() );
}

TEST( OptimizeTest, ShouldNotOptimizeIfPathIsToShort ) {
    Part part;
    part.append( Command( "G02 X1.0 Y0.0 I1.0 J0.0 Z-1.0" ) );
    part.append( Command( "G03 X0.0 Y1.0 I0.0 J1.0 Z-2.0" ) );
    part.append( Command( "G02 X0.0 Y-1.0 I0.0 J-1.0" ) );
    part.append( Command( "G01 X0.0 Y0.0" ) );
    part.append( Command( "G02 X1.0 Y1.0 I1.0 J0.0 Z-3.0" ) );
    part.append( Command( "G03 X0.0 Y0.0 I0.0 J1.0 Z-4.0" ) );
    part.append( Command( "G02 X0.0 Y-1.0 I0.0 J-1.0" ) );

    auto res = optimizePart( part, Position() );
    ASSERT_EQ( res.toString(), part.toString() );
}
