#include <gcode/File.h>
#include <gcode/Optimize.h>

#include <iostream>
#include <cmath>

std::string format( double duration ){
    return std::to_string( static_cast<long>(floor(duration)) ) + ":" + std::to_string( static_cast<long>((duration - floor(duration))*60.0) ) + "min";
}

int main( int argc, char** argv ){

    if( argc != 2 ){
        std::cout << "Missing GCode file, usage: GCodeOptimizer <file>" << std::endl;
        exit(1);
    }

    std::string filename( argv[1] );
    std::cout << "Reading file: " << filename << std::endl;
    gcode::File file( filename );

    std::cout << "Starting optimization of file" << std::endl;
    gcode::Position pos;
    double origDuration = file.duration();
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = gcode::optimizePart( file[idx], pos );
        pos = endPosition( file[idx], pos );
    }

    std::cout << "Estimated duration of original File: " << format(origDuration) << std::endl;
    std::cout << "Estimated duration of optimized File: " << format(file.duration()) << std::endl;
    std::cout << "Saving result to optimized.nc" << std::endl;
    file.save( "optimized.nc" );
}