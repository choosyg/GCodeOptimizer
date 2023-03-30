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
        
        Position pos;
        for( size_t idx=0; idx<file.size(); ++idx ){
            file[ idx ] = gcode::optimizePart( file[idx], pos );
            pos = endPosition( file[idx], pos );
        }
        ASSERT_NEAR( file.duration(), expectedDuration, 1e-5 );

        //file.save( dir + "expected_"+filename );
        File expected( dir + "expected_"+filename );
        ASSERT_EQ( file.size(), expected.size() );
        for( size_t idx=0; idx<file.size(); ++idx ){
            EXPECT_EQ( file[idx].toString(), expected[idx].toString() );
        }
    }
};

TEST_F( FileTest, ShouldOptimizeKreisKlein ) {
    testFile("KreisKlein.nc", 0.153418 );
}

TEST_F( FileTest, ShouldOptimizeKreisGross ) {
    testFile("KreisGross.nc", 2.529321 );
}

TEST_F( FileTest, ShouldOptimizeKreisGrossAnbindungen ) {
    testFile("KreisGrossAnbindungen.nc", 2.759502 );
}

TEST_F( FileTest, ShouldOptimizeRechteck ) {
    testFile("Rechteck.nc", 3.604000 );
}

TEST_F( FileTest, ShouldOptimizeRechteckAnbindungen ) {
    testFile("RechteckAnbindungen.nc", 3.8251235 );
}

TEST_F( FileTest, ShouldOptimizeSchraube ) {
    testFile("Schraube.nc", 0.454307 );
}

TEST_F( FileTest, ShouldOptimizeSchrauben ) {
    testFile("Schrauben.nc", 5.332719 );
}