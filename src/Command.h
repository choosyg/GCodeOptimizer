#pragma once

#include "Position.h"

#include <string>
#include <map>
#include <vector>

class Command {
    public:
      Command( const std::string& str, const Position& before );

      bool isEmpty() const;
      bool isComment() const;
      bool hasKey( char c ) const;

      const Position& setValue( char c, const std::string& value );

      const Position& before() const;
      const Position& after() const;

      std::string toString() const;

    private: 
      struct Param{ 
        Param( char k, const std::string& v ) : key(k), value(v){}
        char key; 
        std::string value;
      };
       
      std::string comment_;
      
      std::vector<Param> params_;
      Position before_, after_;
};