#include "Command.h"

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
      : str_( str ), before_( before ), after_( before )
{
    //Ignore empty lines and comments
    if( str.empty() || str[0] == '(' ){
        return;
    }
    auto params = split( str, ' ' );
    for( const auto& p : params ){
        auto d = std::stod( p.substr(1) );
        map_[p[0]] = d;
        params_.push_back( Param() );
        params_.back().key = p[0];
        params_.back().value = d;
    }

    if( hasKey('X') ) after_.x=value('X');
    if( hasKey('Y') ) after_.y=value('Y');
    if( hasKey('Z') ) after_.z=value('Z');
}

bool Command::isEmpty() const{
    return str_.empty();
}

bool Command::isComment() const{
    return !str_.empty() && str_[0] == '(';
}

bool Command::hasKey( char c ) const{
    return map_.find(c) != map_.end();
}

const double& Command::value( char c ) const{
    return map_.at( c );
}

void Command::setValue( char c, double value ){
    map_[c] = value;
    bool found = false;
    str_ = "";
    for( size_t i=0; i<params_.size(); ++ i ){
        if( params_[i].key == c ){
            params_[i].value = value;
            found=true;
        }
        str_ += params_[i].key + std::to_string(params_[i].value) + " ";
    }
    if( !found ){
        params_.push_back( Param() );
        params_.back().key = c;
        params_.back().value = value;
        str_ += c + std::to_string(value) + " ";
    }
    //remove trailing " "
    str_.erase( str_.size()-1 );
}

const Position& Command::before() const{
return before_;
}

const Position& Command::after() const {
return after_;
}

void Command::comment(){
str_ = "("+str_+")";
}

std::string Command::toString() const{
return str_;
}