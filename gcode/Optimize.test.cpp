#include "Optimize.h"

#include <gtest/gtest.h>

using namespace gcode;

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

    ASSERT_EQ( res.size(), 8 );
    ASSERT_EQ( res[0].toString(), "G01 X1.100000 Y1.100000 Z-1.100000" );
    ASSERT_EQ( res[1].toString(), "G01 X2.0 Y2.0" ); // Z should not become -2 here!
    ASSERT_EQ( res[2].toString(), "G01 X0.0 Y0.0" );
    ASSERT_EQ( res[3].toString(), "G01 X1.100000 Y1.100000 Z-2.200000" );
    ASSERT_EQ( res[4].toString(), "G01 X2.0 Y2.0" );
    ASSERT_EQ( res[5].toString(), "G01 X0.0 Y0.0" );
    ASSERT_EQ( res[6].toString(), "G01 X1.100000 Y1.100000" );
    ASSERT_EQ( res[7].toString(), "G01 X0.000000 Y0.000000" );
}

TEST( OptimizeTest, ShouldOptimizeMinimumCircular ) {
    Part part;
    part.append( Command( "G02 X1.0 Y0.0 I0.0 J-1.0 Z-1.0" ) );//Rotation um 0/0 im Uhrzeigersinn von 0 auf 3 Uhr
    part.append( Command( "G03 X0.0 Y1.0 I-1.0 J0.0 Z-1.1" ) );//Gegen der Uhrzeigersinn von 3 auf 0 Uhr
    part.append( Command( "G02 X0.0 Y-1.0 I0.0 J-1.0" ) );// Im Uhrzeigersinn von 0 auf 6 Uhr
    part.append( Command( "G01 X0.0 Y1.0" ) ); //Linear wieder auf 0 Uhr
    part.append( Command( "G02 X1.0 Y0.0 I0.0 J-1.0 Z-2.0" ) );
    part.append( Command( "G03 X0.0 Y1.0 I-1.0 J0.0 Z-2.2" ) );
    part.append( Command( "G02 X0.0 Y-1.0 I0.0 J-1.0" ) );
    part.append( Command( "G01 X0.0 Y1.0" ) );

    auto res = optimizePart( part, Position(0.0,1.0) );

    ASSERT_EQ( res.size(), 8 );
    EXPECT_EQ( res[0].toString(), "G02 X0.987688 Y-0.156434 Z-1.100000 I0.0 J-1.0" );
    EXPECT_EQ( res[1].toString(), "G02 X0.0 Y-1.0 I-0.987688 J0.156434" );
    EXPECT_EQ( res[2].toString(), "G01 X0.0 Y1.0" );
    EXPECT_EQ( res[3].toString(), "G02 X0.987688 Y-0.156434 Z-2.200000 I0.0 J-1.0" );
    EXPECT_EQ( res[4].toString(), "G02 X0.0 Y-1.0 I-0.987688 J0.156434" );
    EXPECT_EQ( res[5].toString(), "G01 X0.0 Y1.0" );
    EXPECT_EQ( res[6].toString(), "G02 X0.987688 Y-0.156434 I0.0 J-1.0" );
    EXPECT_EQ( res[7].toString(), "G03 X0.000000 Y1.000000 I-0.987688 J0.156434" );
}

TEST( OptimizeTest, ShouldOptimizeMinimumCircular2 ) {
    Part part;
    part.append( Command( "G02 X1.0 Y0.0 I0.0 J-1.0 Z-1.0" ) ); //Rotation um 0/0 im Uhrzeigersinn von 0 auf 3 Uhr
    part.append( Command( "G03 X0.0 Y1.0 I-1.0 J0.0 Z-2.0" ) ); //Gegen der Uhrzeigersinn von 3 auf 0 Uhr
    part.append( Command( "G02 X1.0 Y0.0 I0.0 J-1.0" ) ); // Im Uhrzeigersinn von 0 auf 3 Uhr
    part.append( Command( "G01 X1.0 Y1.0" ) ); //Linear wieder auf 1/1
    part.append( Command( "G01 X0.0 Y1.0" ) ); //Linear wieder auf 0 Uhr
    part.append( Command( "G02 X1.0 Y0.0 I0.0 J-1.0 Z-3.0" ) );
    part.append( Command( "G03 X0.0 Y1.0 I-1.0 J0.0 Z-4.0" ) );
    part.append( Command( "G02 X1.0 Y0.0 I0.0 J-1.0" ) );
    part.append( Command( "G01 X1.0 Y1.0" ) );
    part.append( Command( "G01 X0.0 Y1.0" ) );

    auto res = optimizePart( part, Position(0.0,1.0) );

    ASSERT_EQ( res.size(), 14 );
    EXPECT_EQ( res[0].toString(), "G02 X1.0 Y0.0 Z-1.000000 I0.0 J-1.0" );
    EXPECT_EQ( res[1].toString(), "G01 X1.0 Y1.0 Z-1.636620" );
    EXPECT_EQ( res[2].toString(), "G01 X0.429204 Y1.000000 Z-2.000000" );
    EXPECT_EQ( res[3].toString(), "G01 X0.0 Y1.0" );
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

    ASSERT_EQ( res.size(), 10 );
    EXPECT_EQ( res[0].toString(), "G01 X1.0 Y1.0 Z-1.000000" ); 
    EXPECT_EQ( res[1].toString(), "G01 X2.0 Y2.0 Z-2.000000" ); //Note that the second comand has no Z added since -2 was reached before
    EXPECT_EQ( res[2].toString(), "G01 X0.0 Y0.0" ); 
}


TEST( OptimizeTest, ShouldOptimizeEvenIfPathIsToShort ) {
    Part part;
    part.append( Command( "G02 X1.0 Y0.0 I0.0 J-1.0 Z-1.0" ) );
    part.append( Command( "G03 X0.0 Y1.0 I-1.0 J0.0 Z-2.0" ) );
    part.append( Command( "G02 X1.0 Y0.0 I0.0 J-1.0" ) );
    part.append( Command( "G01 X0.0 Y1.0" ) );
    part.append( Command( "G02 X1.0 Y0.0 I0.0 J-1.0 Z-3.0" ) );
    part.append( Command( "G03 X0.0 Y1.0 I-1.0 J0.0 Z-4.0" ) );
    part.append( Command( "G02 X1.0 Y0.0 I0.0 J-1.0" ) );
    part.append( Command( "G01 X0.0 Y1.0" ) );
    
    auto res = optimizePart( part, Position(0.0,1.0) );
    
    ASSERT_EQ( res.size(), 10 ); // note that we have an extra round to maintain dive angle
    EXPECT_EQ( res[0].toString(), "G02 X1.0 Y0.0 Z-1.000000 I0.0 J-1.0" ); 
    EXPECT_EQ( res[1].toString(), "G01 X0.0 Y1.0 Z-1.900316" );
    EXPECT_EQ( res[2].toString(), "G02 X1.0 Y0.0 Z-2.900316 I0.0 J-1.0" ); 
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
    part.append( Command( "G01 X0.0 Y0.0" ) );
    part.append( Command( "G01 X2.0 Y2.0 Z1.0" ) );//this z position change should block optimization
    part.append( Command( "G01 X0.0 Y0.0" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-2.0" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-2.2" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );

    auto res = optimizePart( part, Position() );

    ASSERT_EQ( res.toString(), part.toString() );
}

TEST( OptimizeTest, ShouldNotOptimizeIfTrackEndIsNotTrackStart ) {
    Part part;
    part.append( Command( "G01 X1.0 Y1.0 Z-1.0" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-1.1" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) ); // Track does not end at 0/0
    part.append( Command( "G01 X1.0 Y1.0 Z-2.0" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-2.2" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );

    auto res = optimizePart( part, Position() );

    ASSERT_EQ( res.toString(), part.toString() );
}

TEST( OptimizeTest, ShouldNotOptimizeIfNextDiveBlockIsAtWrongPosition ) {
    Part part;
    part.append( Command( "G01 X1.0 Y1.0 Z-1.0" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-1.1" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-2.0" ) );//Dive does not start at 0/0
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
    part.append( Command( "G01 X0.0 Y0.0" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-3.0" ) );
    part.append( Command( "G01 X2.0 Y2.0 Z-3.1" ) );
    part.append( Command( "G01 X1.0 Y1.0 Z-3.2" ) );
    part.append( Command( "G01 X0.0 Y0.0 Z-4.0" ) );
    part.append( Command( "G01 X2.0 Y2.0" ) );
    part.append( Command( "G01 X0.0 Y0.0" ) );

    auto res = optimizePart( part, Position() );

    ASSERT_EQ( res.toString(), part.toString() );
}

