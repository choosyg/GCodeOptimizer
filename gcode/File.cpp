#include "File.h"

#include <fstream>

namespace gcode{

File::File( const std::string& filename ){
    //Open file
    std::ifstream file(filename);

    //read all commands and split into parts at empty lines
    parts_.emplace_back();
	while (file) {
		std::string line;
		getline(file, line);
        if( line.empty() && !parts_.back().isEmpty() ){
            parts_.emplace_back();
        } else if( !line.empty() ){
            parts_.back().append( Command( line ) );
        }
	}
}

const Part& File::operator[]( size_t idx ) const{
    return parts_[idx];
}

Part& File::operator[]( size_t idx ){
    return parts_[idx];
}

size_t File::size() const{
    return parts_.size();
}

double File::duration() const{
    double d=0.0;
    Position pos;
    for( const auto&p: parts_){
        d += gcode::duration( p, pos );
        pos = endPosition( p, pos );
    }
    return d;
}

void File::save( const std::string& filename ) const{
    std::ofstream os( "optimized.nc" );
    for( const auto& part : parts_ ){
        os << part.toString() << std::endl;
    }
}

}