#pragma once

#include "Position.h"

#include <string>
#include <map>
#include <vector>

namespace gcode{

class Command {
    public:
      Command( const std::string& str );

      bool isComment() const;
      bool hasKey( char c ) const;
      const std::string& value( char c ) const;
      void setValue( char c, const std::string& value );
      void remove( char c );

      std::string toString() const;

    private: 
      struct Param{ 
        Param( char k, const std::string& v ) : key(k), value(v){}
        char key; 
        std::string value;
        bool operator==( const Param& other ) const { return key ==other.key && value == other.value;}
        bool operator!=( const Param& other ) const { return ! this->operator==(other); }
      };
       
      std::string comment_;
      std::vector<Param> params_;
};

bool operator==( const Command& a, const Command& b );
bool operator!=( const Command& a, const Command& b );

Position endPosition( const Command& cmd, const Position& start );
double length( const Command& cmd, const Position& start );

//split command into 2 commands starting at pos, [0] = going percent of the path from pos to some point, [1] continue path to endPosition
std::array< Command, 2 > split( const Command& cmd, const Position& pos, double percent );

Command invert( const Command& cmd, const Position& pos );

}