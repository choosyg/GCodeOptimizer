#pragma once

#include "Command.h"
#include "Position.h"

#include <vector>

namespace gcode{

// Part consists of
// Comment (some text)
// Approach G00...
// DiveBlock
// Track
// DiveBlock
// Track...
class Part{
    public:
        size_t size() const;
        bool isEmpty() const;

        void append( const Command& cmd );
        void append( const Part& part );
        Part subPart( size_t startIdx, size_t endIdx = static_cast<size_t>(-1) ) const;

        const Command& operator[]( size_t idx ) const;
        Command& operator[]( size_t idx );

        std::string toString() const;

    private:
        std::vector< Command > cmds_;
};

bool operator==( const Part& a, const Part& b );
bool operator!=( const Part& a, const Part& b );

Position endPosition( const Part& part, const Position& startPos );
double length( const Part& part, const Position& startPos );
double duration( const Part& part, const Position& startPos );

}