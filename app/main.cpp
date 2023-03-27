#include <gcode/GCodeFile.h>
#include <gcode/Optimize.h>

#include <iostream>

int main( int argc, char** argv ){

    if( argc != 2 ){
        std::cout << "Missing GCode file, usage: GCodeOptimizer <file>" << std::endl;
        exit(1);
    }

    GCodeFile file( argv[1] );

    Position start;
    for( size_t idx=0; idx<file.size(); ++idx ){
        file[ idx ] = optimizePart( file[idx], start );
        start = file[idx].endPosition( start );
    }

    file.save( "optimized.nc" );
}