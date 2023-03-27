#pragma once

#include "Command.h"
#include "Position.h"

#include <vector>

class Part{
    public:
        size_t start = static_cast<size_t>(-1);
        
        size_t end() const;

        size_t size() const;

        void append( const Part& other, size_t begin = 0 );
        void append( const Part& other, size_t begin, size_t end );
        void append( const Command& cmd );

        const Command& operator[]( size_t idx ) const;
        Command& operator[]( size_t idx );

        const Position& before() const;
        const Position& after() const;

		std::string toString() const;

    private:
        std::vector< Command > cmds_;
};