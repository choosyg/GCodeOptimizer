#include "File.h"
#include "Optimize.h"

#include <gtest/gtest.h>

#include <memory>

using namespace gcode;

class FileTest : public ::testing::Test
{
    protected:
    virtual void SetUp()
    { 
    }

    virtual void TearDown()
    {
    }

    void testFile( const std::string& filename, double expectedDuration ){
        std::string dir(TEST_DATA_DIR);
        File file( dir + filename );

        auto duration = file.duration();
        
        Position pos;
        for( size_t idx=0; idx<file.size(); ++idx ){
            file[ idx ] = gcode::optimizePart( file[idx], pos );
            pos = endPosition( file[idx], pos );
        }
        EXPECT_NEAR( file.duration(), expectedDuration, 1e-5 );
        EXPECT_LT( file.duration(), duration );

        //file.save( dir + "expected_"+filename );
        File expected( dir + "expected_"+filename );
        ASSERT_EQ( file.size(), expected.size() );
        for( size_t idx=0; idx<file.size(); ++idx ){
            EXPECT_EQ( expected[idx].toString(), file[idx].toString() );
        }
    }
};

TEST_F( FileTest, ShouldOptimizeKreisKlein ) {
    testFile("KreisKlein.nc", 0.171635 );
}

TEST_F( FileTest, ShouldOptimizeKreisGross ) {
    testFile("KreisGross.nc", 2.567130 );
}

TEST_F( FileTest, ShouldOptimizeKreisGrossAnbindungen ) {
    testFile("KreisGrossAnbindungen.nc", 2.787858 );
}

TEST_F( FileTest, ShouldOptimizeRechteck ) {
    testFile("Rechteck.nc", 3.641809 );
}

TEST_F( FileTest, ShouldOptimizeRechteckAnbindungen ) {
    testFile("RechteckAnbindungen.nc", 3.853480 );
}

TEST_F( FileTest, ShouldOptimizeSchraube ) {
    testFile("Schraube.nc", 0.454307 );
}

TEST_F( FileTest, ShouldOptimizeSchrauben ) {
    testFile("Schrauben.nc", 5.332719 );
}