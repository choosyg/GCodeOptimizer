#include "Command.h"

#include <gtest/gtest.h>

#include <cmath>

using namespace gcode;

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
    auto p = endPosition( cmd, Position() );
    ASSERT_EQ( 1.0, p.x );
    ASSERT_EQ( 2.0, p.y );
    ASSERT_EQ( 3.0, p.depth );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForLinearMovementX ) {
    Position pos( 0.0, 0.0, 0.0 );
    Command cmd( "G00 X1.0");
    ASSERT_EQ( 1.0, length( cmd, pos ) );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForLinearMovementY ) {
    Position pos( 0.0, 0.0, 0.0 );
    Command cmd( "G00 Y1.0");
    ASSERT_EQ( 1.0, length( cmd, pos ) );
}

TEST( CommandTest, ShouldIgnoreLengthForLinearMovementZ ) {
    Position pos( 0.0, 0.0, 0.0 );
    Command cmd( "G00 Z1.0");
    ASSERT_EQ( 0.0, length( cmd, pos ) );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForLinearMovementXYZ ) {
    Position pos( 0.0, 0.0, 0.0 );
    Command cmd( "G00 X1.0 Y1.0 Z1.0");
    ASSERT_EQ( sqrt(2.0), length( cmd, pos ) ); //z should be ignored
}

TEST( CommandTest, ShouldCalculateCorrectLengthForLinearMovementG1XYZ ) {
    Position pos( 0.0, 0.0, 0.0 );
    Command cmd( "G01 X1.0 Y1.0 Z1.0");
    ASSERT_EQ( sqrt(2.0), length( cmd, pos ) ); //z should be ignored
}

TEST( CommandTest, ShouldCalculateCorrectLengthForG2_90Degree ) {
    Position pos( 0.0, 1.0, 0.0 );
    Command cmd( "G02 X1.0 Y0.0 I0.0 J-1.0");
    ASSERT_NEAR( M_PI/2.0, length( cmd, pos ), 0.0000001 );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForG2_180Degree ) {
    Position pos( 0.0, 1.0, 0.0 );
    Command cmd( "G02 X0.0 Y-1.0 I0.0 J-1.0");
    ASSERT_NEAR( M_PI, length( cmd, pos ), 0.0000001 );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForG2_270Degree ) {
    Position pos( 0.0, 1.0, 0.0 );
    Command cmd( "G02 X-1.0 Y0.0 I0.0 J-1.0");
    ASSERT_NEAR( M_PI/2.0*3.0, length( cmd, pos ), 0.0000001 );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForG3_270Degree ) {
    Position pos( 0.0, 1.0, 0.0 );
    Command cmd( "G03 X1.0 Y0.0 I0.0 J-1.0");
    ASSERT_NEAR( M_PI/2.0*3.0, length( cmd, pos ), 0.0000001 );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForG3_180Degree ) {
    Position pos( 0.0, 1.0, 0.0 );
    Command cmd( "G03 X0.0 Y-1.0 I0.0 J-1.0");
    ASSERT_NEAR( M_PI, length( cmd, pos ), 0.0000001 );
}

TEST( CommandTest, ShouldCalculateCorrectLengthForG3_90Degree ) {
    Position pos( 0.0, 1.0, 0.0 );
    Command cmd( "G03 X-1.0 Y0.0 I0.0 J-1.0");
    ASSERT_NEAR( M_PI/2.0, length( cmd, pos ), 0.0000001 );
}

TEST( CommandTest, ShallCompare ) {
    Command cmd1( "G00 X1.0 Y2.0 Z3.0");
    Command cmd2( "G00 X1.0 Y2.0 Z3.0");
    Command cmd3( "G00 X1.0 Y2.0 Z3.1");
    ASSERT_EQ( cmd1, cmd2 );
    ASSERT_NE( cmd1, cmd3 );
}

TEST( CommandTest, ShallSplitFast ) {
    Position pos;
    Command cmd( "G00 X2.0, Y4.0 Z1.0");
    auto s = split( cmd, pos, 0.5 );
    ASSERT_EQ( s[0].toString(), "G00 X1.000000 Y2.000000 Z0.500000" );
    ASSERT_EQ( s[1].toString(), cmd.toString() );
}

TEST( CommandTest, ShallSplitLinear ) {
    Position pos;
    Command cmd( "G01 X2.0, Y4.0 Z1.0");
    auto s = split( cmd, pos, 0.5 );
    ASSERT_EQ( s[0].toString(), "G01 X1.000000 Y2.000000 Z0.500000" );
    ASSERT_EQ( s[1].toString(), cmd.toString() );
}

TEST( CommandTest, ShallSplitClockwise ) {
    Position pos(0.0,1.0,0.0);
    Command cmd( "G02 X0.0, Y-1.0 Z1.0 I0 J-1");
    auto s = split( cmd, pos, 0.5 );
    ASSERT_EQ( s[0].toString(), "G02 X1.000000 Y0.000000 Z0.500000 I0 J-1" );
    ASSERT_EQ( s[1].toString(), "G02 X0.0, Y-1.0 Z1.0 I-1.000000 J-0.000000" );
}

TEST( CommandTest, ShallSplitCounterClockwise ) {
    Position pos(1.0,0.0,0.0);
    Command cmd( "G03 X-1.0, Y0.0 Z1.0 I-1 J0");
    auto s = split( cmd, pos, 0.5 );
    ASSERT_EQ( s[0].toString(), "G03 X0.000000 Y1.000000 Z0.500000 I-1 J0" );
    ASSERT_EQ( s[1].toString(), "G03 X-1.0, Y0.0 Z1.0 I-0.000000 J-1.000000" );
}

TEST( CommandTest, ShallInvertFast ) {
    Position pos( 2.0,3.0,4.0);
    Command cmd( "G00 X2.0, Y4.0 Z1.0");
    auto inv = invert( cmd, pos );
    ASSERT_EQ( inv.toString(), "G00 X2.000000 Y3.000000 Z4.000000" );
}

TEST( CommandTest, ShallInvertLinear ) {
    Position pos( 2.0,3.0,4.0);
    Command cmd( "G01 X2.0, Y4.0 Z1.0");
    auto inv = invert( cmd, pos );
    ASSERT_EQ( inv.toString(), "G01 X2.000000 Y3.000000 Z4.000000" );
}

TEST( CommandTest, ShallInvertClockwise ) {
    Position pos(0.0,1.0,0.0);
    Command cmd( "G02 X1.000000 Y0.000000 Z1.000000 I0.000000 J-1.000000");
    auto endPos = endPosition( cmd, pos );
    auto inv = invert( cmd, pos );
    ASSERT_EQ( inv.toString(), "G03 X0.000000 Y1.000000 Z0.000000 I-1.000000 J0.000000" );
    auto inv2 = invert( inv, endPos );
    ASSERT_EQ( inv2.toString(), cmd.toString() );
}