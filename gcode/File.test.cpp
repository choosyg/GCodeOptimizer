#include "File.h"
#include "Optimize.h"

#include <gtest/gtest.h>

using namespace gcode;

TEST( FileTest, ShouldOptimizeKreisKlein ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"KreisKlein.nc" );

    ASSERT_EQ( file.size(), 6 );
    ASSERT_NEAR( file.duration(), 0.209402, 1e-5 ); // 12 Sekunden

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 0.153418, 1e-5 ); // 6 Sekunden
}

TEST( FileTest, ShouldOptimizeKreisGross ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"KreisGross.nc" );

    ASSERT_EQ( file.size(), 6 );
    ASSERT_NEAR( file.duration(), 2.737268, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 2.529321, 1e-5 );
}

TEST( FileTest, ShouldOptimizeKreisGrossAnbindungen ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"KreisGrossAnbindungen.nc" );

    ASSERT_EQ( file.size(), 6 );
    ASSERT_NEAR( file.duration(), 2.910736, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 2.759502, 1e-5 );
}

TEST( FileTest, ShouldOptimizeRechteck ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"Rechteck.nc" );

    ASSERT_EQ( file.size(), 6 );
    ASSERT_NEAR( file.duration(), 3.811948, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 3.604000, 1e-5 );
}

TEST( FileTest, ShouldOptimizeRechteckAnbindungen ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"RechteckAnbindungen.nc" );

    ASSERT_EQ( file.size(), 6 );
    ASSERT_NEAR( file.duration(), 3.976358, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 3.8251235, 1e-5 );
}

TEST( FileTest, ShouldOptimizeSchraube ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"Schraube.nc" );

    ASSERT_EQ( file.size(), 7 );
    ASSERT_NEAR( file.duration(), 0.613106, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 0.454307, 1e-5 );
}

TEST( FileTest, ShouldOptimizeSchrauben ) {
    std::string dir(TEST_DATA_DIR);
    File file( dir+"Schrauben.nc" );

    ASSERT_EQ( file.size(), 29 );
    ASSERT_NEAR( file.duration(), 7.238299, 1e-5 );

    Position pos;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    ASSERT_NEAR( file.duration(), 5.332719, 1e-5 );
}