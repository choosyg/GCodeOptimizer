#include "Optimize.h"

#include <iostream>
#include <fstream>

int main( int argc, char** argv ){

    if( argc != 2 ){
        std::cout << "Missing GCode file, usage: GCodeOptimizer <file>" << std::endl;
        exit(1);
    }

    auto complete = readComplete( argv[1] );
    auto parts = findParts( complete );

    Part result;
    size_t idx = 0;
    for( const auto& part: parts ){
        result.append( complete, idx, part.start );
        result.append( optimizePart( part ) );
        idx = part.end();
    }
    result.append( complete, idx );

    std::ofstream( "result.nc" ) << result.toString() << std::endl;
}