#include <gcode/GCodeFile.h>
#include <gcode/Optimize.h>

#include <iostream>

int main( int argc, char** argv ){

    if( argc != 2 ){
        std::cout << "Missing GCode file, usage: GCodeOptimizer <file>" << std::endl;
        exit(1);
    }

    std::string filename( argv[1] );
    std::cout << "Reading file: " << filename << std::endl;
    GCodeFile file( filename );

    std::cout << "Starting optimization of file" << std::endl;
    Position start;
    double origDuration = 0.0;
    double optDuration = 0.0;
    for( size_t idx=0; idx<file.size(); ++idx ){
        origDuration += file[idx].pathDuration( start );
        file[ idx ] = optimizePart( file[idx], start );
        optDuration += file[idx].pathDuration( start );
        start = file[idx].endPosition( start );
    }

    std::cout << "Estimated duration of original File: " << origDuration << std::endl;
    std::cout << "Estimated duration of optimized File: " << optDuration << std::endl;
    std::cout << "Saving result to optimized.nc" << std::endl;
    file.save( "optimized.nc" );
}