#include "Command.h"

#include <algorithm>

namespace{
    std::vector< std::string > split( const std::string& str, char d ){
        size_t start = 0;
        size_t end = str.find( d );
        std::vector< std::string > res;

        while( end != std::string::npos ) {
            res.push_back( str.substr( start, end-start ) );
            start = end + 1;
            end = str.find( d, start );
        }
        res.push_back( str.substr( start ) );

        return res;
    }
}

Command::Command( const std::string& str, const Position& before )
      : before_( before ), after_( before )
{
    //Ignore non-GCode lines, eg. empty lines, comments, MCode
    if( str.empty() || str[0] == '(' ){
        comment_ = str;
        return;
    }
    
    auto params = split( str, ' ' );
    for( const auto& p : params ){
        setValue( p[0], p.substr(1) );
    }
}

bool Command::isEmpty() const{
    return comment_.empty() && params_.empty();
}

bool Command::isComment() const{
    return !comment_.empty();
}

bool Command::hasKey( char c ) const{
    auto it = std::find_if( params_.begin(), params_.end(), [&](const Param& p){return p.key==c;});
    return it != params_.end();
}

const Position& Command::setValue( char c, const std::string& value ){
    auto it = std::find_if( params_.begin(), params_.end(), [&](const Param& p){return p.key==c;});
    if( it == params_.end() ){
        params_.emplace_back( c, value );
    } else {
        it->value = value;
    }
    if( c == 'X' ) after_.x = std::stod( value );
    if( c == 'Y' ) after_.y = std::stod( value );
    if( c == 'Z' ) after_.z = std::stod( value );
    return after_;
}

const Position& Command::before() const{
    return before_;
}

const Position& Command::after() const {
    return after_;
}

std::string Command::toString() const{
    std::string str = comment_;

    if( !params_.empty() ){
        //Add params, maintain default order
        auto copy = params_;
        std::vector<char> order = { 'G', 'M', 'X', 'Y', 'Z', 'I', 'J', 'F', 'S' };
        for( const auto& c: order ){
            auto it = std::find_if( copy.begin(), copy.end(), [&](const Command::Param& p){return p.key==c;});
            if( it != copy.end() ){
                str += it->key + it->value + " ";
                copy.erase( it );
            }
        }
        //Add unknown params
        for( const auto & p: copy ){
            str += p.key + p.value + " ";
        }
        //remove trailing " "
        str.erase( str.size()-1 );
    }
    return str;
}