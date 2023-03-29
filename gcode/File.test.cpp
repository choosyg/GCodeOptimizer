#include "File.h"
#include "Optimize.h"

#include <gtest/gtest.h>

using namespace gcode;

TEST( FileTest, ShouldOptimizeKreisKlein ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"KreisKlein.nc" );

    ASSERT_EQ( file.size(), 6 );
    ASSERT_NEAR( file.duration(), 0.159402, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 0.1012504, 1e-5 );
}

TEST( FileTest, ShouldOptimizeKreisGross ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"KreisGross.nc" );

    ASSERT_EQ( file.size(), 6 );
    ASSERT_NEAR( file.duration(), 2.687268, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 2.470327, 1e-5 );
}

TEST( FileTest, ShouldOptimizeKreisGrossAnbindungen ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"KreisGrossAnbindungen.nc" );

    ASSERT_EQ( file.size(), 6 );
    ASSERT_NEAR( file.duration(), 2.860736, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 2.836582, 1e-5 );
}

TEST( FileTest, ShouldOptimizeRechteck ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"Rechteck.nc" );

    ASSERT_EQ( file.size(), 6 );
    ASSERT_NEAR( file.duration(), 3.761948, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 3.544779, 1e-5 );
}

TEST( FileTest, ShouldOptimizeRechteckAnbindungen ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"RechteckAnbindungen.nc" );

    ASSERT_EQ( file.size(), 6 );
    ASSERT_NEAR( file.duration(), 3.926358, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 3.765671, 1e-5 );
}

TEST( FileTest, ShouldOptimizeSchraube ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"Schraube.nc" );

    ASSERT_EQ( file.size(), 7 );
    ASSERT_NEAR( file.duration(), 0.563106, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 0.393139, 1e-5 );
}

TEST( FileTest, ShouldOptimizeSchrauben ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"Schrauben.nc" );

    ASSERT_EQ( file.size(), 29 );
    ASSERT_NEAR( file.duration(), 7.188299, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 5.147267, 1e-5 );
}