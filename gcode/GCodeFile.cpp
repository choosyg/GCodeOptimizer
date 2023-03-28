#include "GCodeFile.h"

#include <fstream>

GCodeFile::GCodeFile( const std::string& filename ){
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

const Part& GCodeFile::operator[]( size_t idx ) const{
    return parts_[idx];
}

Part& GCodeFile::operator[]( size_t idx ){
    return parts_[idx];
}

size_t GCodeFile::size() const{
    return parts_.size();
}

void GCodeFile::save( const std::string& filename ) const{
    std::ofstream os( "optimized.nc" );
    for( const auto& part : parts_ ){
        os << part.toString() << std::endl;
    }
}