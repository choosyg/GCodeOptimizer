#pragma once

#include "Part.h"

class GCodeFile{
    public:
        GCodeFile( const std::string& filename );

        const Part& operator[]( size_t idx ) const;
        Part& operator[]( size_t idx );
        size_t size() const;

        void save( const std::string& filename ) const;

    private:
        std::vector<Part> parts_;
};