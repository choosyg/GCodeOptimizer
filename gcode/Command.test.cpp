#include "Command.h"

#include <gtest/gtest.h>

#include <cmath>

TEST( CommandTest, ShallRecognizeComment ) {
    Command cmd( "(Test)");
    ASSERT_TRUE( cmd.isComment() );
}

TEST( CommandTest, ShallRecognizeKeysAndValues ) {
    Command cmd( "G00 X01");
    ASSERT_TRUE( cmd.hasKey('X') );
    ASSERT_TRUE( cmd.hasKey('G') );
    ASSERT_EQ( cmd.value('X'), "01");
    ASSERT_EQ( cmd.value('G'), "00");

    cmd.setValue( 'X', "1.0" );
    cmd.setValue( 'Y', "2.0" );
    ASSERT_EQ( cmd.value('X'), "1.0");
    ASSERT_EQ( cmd.value('Y'), "2.0");

    cmd.remove('G');
    ASSERT_FALSE( cmd.hasKey('G') );
    ASSERT_TRUE( cmd.hasKey('X') );
    ASSERT_TRUE( cmd.hasKey('Y') );

    ASSERT_EQ( "X1.0 Y2.0", cmd.toString() );
}

TEST( CommandTest, ShallFindEndPosition ) {
    Command cmd( "G00 X1.0 Y2.0 Z3.0");
    auto p = cmd.endPosition( Position() );
    ASSERT_EQ( 1.0, p.x );
    ASSERT_EQ( 2.0, p.y );
    ASSERT_EQ( 3.0, p.depth );
}

TEST( CommandTest, ShallCompare ) {
    Command cmd1( "G00 X1.0 Y2.0 Z3.0");
    Command cmd2( "G00 X1.0 Y2.0 Z3.0");
    Command cmd3( "G00 X1.0 Y2.0 Z3.1");
    ASSERT_EQ( cmd1, cmd2 );
    ASSERT_NE( cmd1, cmd3 );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForLinearMovementX ) {
    Position pos( 0.0, 0.0, 0.0 );
    Command cmd( "G00 X1.0");
    ASSERT_EQ( 1.0, cmd.pathLength( pos ) );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForLinearMovementY ) {
    Position pos( 0.0, 0.0, 0.0 );
    Command cmd( "G00 Y1.0");
    ASSERT_EQ( 1.0, cmd.pathLength( pos ) );
}

TEST( CommandTest, ShouldIgnoreLengthForLinearMovementZ ) {
    Position pos( 0.0, 0.0, 0.0 );
    Command cmd( "G00 Z1.0");
    ASSERT_EQ( 0.0, cmd.pathLength( pos ) );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForLinearMovementXYZ ) {
    Position pos( 0.0, 0.0, 0.0 );
    Command cmd( "G00 X1.0 Y1.0 Z1.0");
    ASSERT_EQ( sqrt(2.0), cmd.pathLength( pos ) ); //z should be ignored
}

TEST( CommandTest, ShouldCalculateCorrectLengthForLinearMovementG1XYZ ) {
    Position pos( 0.0, 0.0, 0.0 );
    Command cmd( "G01 X1.0 Y1.0 Z1.0");
    ASSERT_EQ( sqrt(2.0), cmd.pathLength( pos ) ); //z should be ignored
}

TEST( CommandTest, ShouldCalculateCorrectLengthForG2_90Degree ) {
    Position pos( 0.0, 1.0, 0.0 );
    Command cmd( "G02 X1.0 Y0.0 I1.0 J0.0");
    ASSERT_NEAR( M_PI/2.0, cmd.pathLength( pos ), 0.0000001 );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForG2_180Degree ) {
    Position pos( 0.0, 1.0, 0.0 );
    Command cmd( "G02 X0.0 Y-1.0 I0.0 J-1.0");
    ASSERT_NEAR( M_PI, cmd.pathLength( pos ), 0.0000001 );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForG2_270Degree ) {
    Position pos( 0.0, 1.0, 0.0 );
    Command cmd( "G02 X-1.0 Y0.0 I-1.0 J0.0");
    ASSERT_NEAR( M_PI/2.0*3.0, cmd.pathLength( pos ), 0.0000001 );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForG3_90Degree ) {
    Position pos( 0.0, 1.0, 0.0 );
    Command cmd( "G03 X1.0 Y0.0 I1.0 J0.0");
    ASSERT_NEAR( M_PI/2.0*3.0, cmd.pathLength( pos ), 0.0000001 );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForG3_180Degree ) {
    Position pos( 0.0, 1.0, 0.0 );
    Command cmd( "G03 X0.0 Y-1.0 I0.0 J-1.0");
    ASSERT_NEAR( M_PI, cmd.pathLength( pos ), 0.0000001 );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForG3_270Degree ) {
    Position pos( 0.0, 1.0, 0.0 );
    Command cmd( "G03 X-1.0 Y0.0 I-1.0 J0.0");
    ASSERT_NEAR( M_PI/2.0, cmd.pathLength( pos ), 0.0000001 );
}