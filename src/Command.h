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

      const double& value( char c ) const;
      void setValue( char c, double value );

      const Position& before() const;
      const Position& after() const;

      void comment();

      std::string toString() const;

    private:
        std::string str_;
        std::map< char, double > map_;
        struct Param{ char key; double value;};
        std::vector<Param> params_;
        Position before_, after_;
};