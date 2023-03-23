#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <string.h>
#include <vector>

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

struct Position{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    bool operator== ( const Position& other ) const{
        //z is ignored!
        return x == other.x && y == other.y;
    }
    bool operator!= ( const Position& other ) const{
        //z is ignored!
        return !this->operator==(other);
    }
};

class Command {
    public:
      Command( const std::string& str, const Position& before )
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

      bool isEmpty() const{
        return str_.empty();
      }

      bool isComment() const{
        return !str_.empty() && str_[0] == '(';
      }

      bool hasKey( char c ) const{
        return map_.find(c) != map_.end();
      }

      const double& value( char c ) const{
        return map_.at( c );
      }

      void setValue( char c, double value ){
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

      const Position& before() const{
        return before_;
      }

      const Position& after() const {
        return after_;
      }

      void comment(){
        str_ = "("+str_+")";
      }

      std::string toString() const{
        return str_;
      }

    private:
        std::string str_;
        std::map< char, double > map_;
        struct Param{ char key; double value;};
        std::vector<Param> params_;
        Position before_, after_;
};

class Part{
    public:
    size_t start = static_cast<size_t>(-1);
    
    size_t end() const{
        return start+size();
    }

    size_t size() const {
        return cmds_.size();
    }

    void append( const Part& other, size_t begin = 0 ){
        return append( other, begin, other.size() );
    }

    void append( const Part& other, size_t begin, size_t end ){
        for( size_t i=begin; i<end; ++i ) { 
            cmds_.push_back( other[i] );
        }
    }

    void append( const Command& cmd ){
        cmds_.push_back( cmd );
    }

    const Command& operator[]( size_t idx ) const{
        return cmds_[idx];
    }
    
    Command& operator[]( size_t idx ){
        return cmds_[idx];
    }

    const Position& before() const{
        return cmds_.front().before();
      }

      const Position& after() const {
        return cmds_.back().after();
      }

    private:
    std::vector< Command > cmds_;
};

std::vector< Part > findParts( const Part& complete ){
    std::vector< Part > parts;
    for( size_t i=0 ; i<complete.size(); ++i ){
        while( i<complete.size() && (complete[i].isEmpty() || complete[i].isComment()) ){
            ++i;
        }
        if( i==complete.size() ){
            break;
        }
        size_t start = i;
        
        while( i<complete.size() && !complete[i].isEmpty() && !complete[i].isComment() ){
            ++i;
        }
        Part part;
        part.start = start;
        part.append( complete, start, i );
        parts.push_back( part );
    }
    return parts;
}

struct Block{
    Block( size_t s, size_t e, bool c = false ) : start(s), end(e), colinear(c){};
    size_t start=0;
    size_t end=0;
    bool colinear = false;
};

bool isColinear( const Command& cmd1, const Command& cmd2 ){
    //The command 2 has to go back to position before command 1
    return cmd1.before() != cmd1.after() && cmd1.before() == cmd2.after();
}

std::vector<Block> splitIfContainsColinearRows( const Part& part, const Block& block ){
    size_t start = block.start;
    size_t end = block.start+1;
    while( end < block.end ){
        //if [start,end] is colinear, expand while colinear
        while( start>=block.start && end<block.end && isColinear(part[start], part[end]) ){
            --start;
            ++end;
        }

        //if [start, end] was not colinear try next pair
        if( end == start+1 ){
            ++start;
            ++end;
        }
        else {
            ++start; //make sure start points to the first command
            break;
        }
    }

    std::vector<Block> result;
    if( end != start+1 ){
        std::cout << "Found interesting Block: " << start << " , " << end << std::endl;
        if( start>block.start ) result.emplace_back( block.start, start );
        result.emplace_back( start, end, true );
        if( end<block.end ) result.emplace_back( end, block.end );
    } else {
        result.push_back( block );
    }
    return result;
}

std::vector< Block > findZBlocks( const Part& part ){
    std::vector< Block > blocks;
    for( size_t i=0 ; i<part.size(); ++i ){
        while( i<part.size() && !part[i].hasKey('Z') ){
            ++i;
        }
        if( i==part.size() ){
            break;
        }
        size_t start = i;
        while( i<part.size() && part[i].hasKey('Z') ){
            ++i;
        }
        
        auto bs = splitIfContainsColinearRows( part, Block( start, i ) );
        blocks.insert( blocks.end(), bs.begin(), bs.end() );
    }
    return blocks;
}



Part optimizePart( const Part& part ){
    auto blocks = findZBlocks( part );

    Part result;
    result.start = part.start;
    size_t idx = 0;

    for( size_t i=1; i<blocks.size(); ++i ){
        const Block& a = blocks[i-1];
        const Block& b = blocks[i];
        /*
        Colinear blocks can only be optimized if 
        * The next z block is colinear as well and at the same positions 
        -> sonst ggf. eine Anbindung in der Mitte (z achse wird vor der Anbindung hochgefahren)
        -> der letzte Block muss den Rückweg machen, sonst bleibt die Rampe übrig
        */
        if( !a.colinear || !b.colinear || a.end-a.start!=b.end-b.start ){
            std::cout << "Skipping following lines are not matching" << std::endl;
            continue;
        }

        /*
        Sind also beide colinear und gleich lang -> ersten Block optimieren
        Die erste Hälfte der Commands im Block müssen nachfolgend nochmal ohne Z anpassung angehängt sein
        Die letzte Zeile kann abweichen, da evtl. die dort zu fahrende strecke unterbrochen wurde weil der umkehrpunkt erreicht war
        */
        bool isValid = true;
        size_t steps = (a.end-a.start)/2;
        for( size_t i=0; i<steps-1 ; ++i ){
            if( a.end+i>=part.size() || part[ a.start+i ].after() != part[a.end+i].after() ) {
                std::cout << "Skipping no trailing lines" << std::endl;
                std::cout << "  " << part[ a.start+i ].after().x << " , " << part[ a.start+i ].after().y << std::endl;
                std::cout << "  " << part[ a.end+i ].after().x << " , " << part[ a.end+i ].after().y << std::endl;
                isValid = false;
            }
        }
        if( !isValid ){
            continue;
        }

        /* 
        So, nun können wir 
        - Den Block weglassen und die Folgezeilen das Z anpassen
        */
        result.append( part, idx, a.start );
        idx = a.end+steps;
        double startZ = part[a.start].before().z;
        for( size_t i=0; i< steps; ++i ){
            
            //Gehe doppelt so tief wie ursprünglich auf diesem Abschnitt
            double z = part[a.start+i].after().z;
            double newz = z + z-startZ;
            //sicherstellen, dass wir am Ende den korrekten Wert haben ohne Rechenfehler
            if( i==steps-1 ){
                newz = part[a.end-1].after().z;
            }

            
            Command cmd = part[ a.end+i ];
            cmd.setValue( 'Z', z + z-startZ );
            result.append( cmd );
        }
    }
    result.append( part, idx );
    return result;
}

Part readComplete(){
    Part complete;
    complete.start = 0;
    std::ifstream file("test.nc");
    Position pos;
    while( file ){
      std::string line;
      getline( file, line );
      complete.append( Command( line, pos ) );
      pos = complete.after();
    }
    return complete;
}

int main( int argc, char** argv ){

    auto complete = readComplete();
    auto parts = findParts( complete );

    Part result;
    size_t idx = 0;
    for( const auto& part: parts ){
        result.append( complete, idx, part.start );
        result.append( optimizePart( part ) );
        idx = part.end();
    }
    result.append( complete, idx );

    std::ofstream os("result.nc");
    for( size_t i=0; i<result.size(); ++i ){
        os << result[i].toString() << std::endl;
    }
}