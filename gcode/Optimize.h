#pragma once

#include "Part.h"

#include <string>
#include <vector>

namespace gcode{

Part optimizePart( const Part& part, const Position& start );

}