#pragma once

#include "Part.h"

namespace gcode{

class File{
    public:
        File( const std::string& filename );

        const Part& operator[]( size_t idx ) const;
        Part& operator[]( size_t idx );
        size_t size() const;

        double duration() const;

        void save( const std::string& filename ) const;

    private:
        std::vector<Part> parts_;
};

}