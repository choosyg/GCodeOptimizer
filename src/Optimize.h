#pragma once

#include "Part.h"

#include <string>
#include <vector>

Part readComplete(std::string filename);
std::vector< Part > findParts(const Part& complete);
Part optimizePart(const Part& part);

