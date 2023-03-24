#include "Part.h"

size_t Part::end() const{
    return start+size();
}

size_t Part::size() const {
    return cmds_.size();
}

void Part::append( const Part& other, size_t begin ){
    return append( other, begin, other.size() );
}

void Part::append( const Part& other, size_t begin, size_t end ){
    for( size_t i=begin; i<end; ++i ) { 
        cmds_.push_back( other[i] );
    }
}

void Part::append( const Command& cmd ){
    cmds_.push_back( cmd );
}

const Command& Part::operator[]( size_t idx ) const{
    return cmds_[idx];
}

Command& Part::operator[]( size_t idx ){
    return cmds_[idx];
}

const Position& Part::before() const{
    return cmds_.front().before();
}

const Position& Part::after() const {
    return cmds_.back().after();
}

std::string Part::toString() const {
    std::string str;
    for( size_t i = 0; i < cmds_.size(); ++i ) {
        str += cmds_[i].toString() + "\r\n";
    }
    return str;
}