#pragma once

#include "Command.h"
#include "Position.h"

#include <vector>
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

        bool operator==( const Part& other ) const;
        bool operator!=( const Part& other ) const;

        Position endPosition(  const Position& start ) const;
        double pathLength( const Position& startPos ) const;
        double pathDuration( const Position& startPos ) const;

		std::string toString() const;

    private:
        std::vector< Command > cmds_;
};