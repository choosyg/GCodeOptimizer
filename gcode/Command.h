#pragma once

#include "Position.h"

#include <string>
#include <map>
#include <vector>

class Command {
    public:
      Command( const std::string& str );

      bool isComment() const;
      bool hasKey( char c ) const;
      const std::string& value( char c ) const;
      void setValue( char c, const std::string& value );
      void remove( char c );

      Position endPosition(  const Position& start ) const;
      double pathLength( const Position& start ) const;

      std::string toString() const;

      bool operator==( const Command& other ) const;
      bool operator!=( const Command& other ) const;

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