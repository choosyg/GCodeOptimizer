#include "Optimize.h"

#include "Command.h"
#include "Position.h"
#include "Part.h"

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <string.h>
#include <vector>

namespace {

	struct Block {
		Block(size_t s, size_t e, bool c = false) : start(s), end(e), colinear(c) {};
		size_t start = 0;
		size_t end = 0;
		bool colinear = false;
	};

	bool isColinear(const Command& cmd1, const Command& cmd2) {
		//The command 2 has to go back to position before command 1
		return cmd1.before() != cmd1.after() && cmd1.before() == cmd2.after();
	}

	std::vector<Block> splitIfContainsColinearRows(const Part& part, const Block& block) {
		size_t start = block.start;
		size_t end = block.start + 1;
		while (end < block.end) {
			//if [start,end] is colinear, expand while colinear
			while (start >= block.start && end < block.end && isColinear(part[start], part[end])) {
				--start;
				++end;
			}

			//if [start, end] was not colinear try next pair
			if (end == start + 1) {
				++start;
				++end;
			}
			else {
				++start; //make sure start points to the first command
				break;
			}
		}

		std::vector<Block> result;
		if (end != start + 1) {
			if (start > block.start) {
				result.emplace_back(block.start, start);
                std::cout << "Identified Z-Block: [" << result.back().start << "," << result.back().end << ")" << std::endl;
			}
			result.emplace_back(start, end, true);
			std::cout << "Identified optimization candidate Z-Block: [" << result.back().start << "," << result.back().end << ")" << std::endl;
            if (end < block.end) {
				result.emplace_back(end, block.end);
                std::cout << "Identified Z-Block: [" << result.back().start << "," << result.back().end << ")" << std::endl;
			}
		}
		else {
			result.push_back(block);
            std::cout << "Identified Z-Block: [" << block.start << "," << block.end << ")" << std::endl;
		}
		return result;
	}

	std::vector< Block > findZBlocks(const Part& part) {
		std::vector< Block > blocks;
		for (size_t i = 0; i < part.size(); ++i) {
			while (i < part.size() && !part[i].hasKey('Z')) {
				++i;
			}
			if (i == part.size()) {
				break;
			}
			size_t start = i;
			while (i < part.size() && part[i].hasKey('Z')) {
				++i;
			}

			auto bs = splitIfContainsColinearRows(part, Block(start, i));
			blocks.insert(blocks.end(), bs.begin(), bs.end());
		}
		return blocks;
	}

}

Part readComplete(std::string filename) {
	Part complete;
	complete.start = 0;
	std::ifstream file(filename);
	Position pos( 0.0, 0.0, 0.0 );
	while (file) {
		std::string line;
		getline(file, line);
		complete.append(Command(line, pos));
		pos = complete.after();
	}
	return complete;
}

std::vector< Part > findParts(const Part& complete) {
	std::vector< Part > parts;
	for (size_t i = 0; i < complete.size(); ++i) {
		while (i < complete.size() && (complete[i].isEmpty() || complete[i].isComment())) {
			++i;
		}
		if (i == complete.size()) {
			break;
		}
		size_t start = i;

		while (i < complete.size() && !complete[i].isEmpty() && !complete[i].isComment()) {
			++i;
		}
		Part part;
		part.start = start;
		part.append(complete, start, i);
		parts.push_back(part);
	}
	return parts;
}



Part optimizePart(const Part& part) {
	auto blocks = findZBlocks(part);

	Part result;
	result.start = part.start;
	size_t idx = 0;

	for (size_t i = 1; i < blocks.size(); ++i) {
		const Block& a = blocks[i - 1];
		const Block& b = blocks[i];
		/*
		Colinear blocks can only be optimized if
		* The next z block is colinear as well and at the same positions
		-> sonst ggf. eine Anbindung in der Mitte (z achse wird vor der Anbindung hochgefahren)
		-> der letzte Block muss den Rückweg machen, sonst bleibt die Rampe übrig
		*/
		if (!a.colinear || !b.colinear || a.end - a.start != b.end - b.start) {
			continue;
		}

		/*
		Sind also beide colinear und gleich lang -> ersten Block optimieren
		Die erste Hälfte der Commands im Block müssen nachfolgend nochmal ohne Z anpassung angehängt sein
		Die letzte Zeile kann abweichen, da evtl. die dort zu fahrende strecke unterbrochen wurde weil der umkehrpunkt erreicht war
		*/
		bool isValid = true;
		size_t steps = (a.end - a.start) / 2;
		for (size_t i = 0; i < steps - 1; ++i) {
			if (a.end + i >= part.size() || part[a.start + i].after() != part[a.end + i].after()) {
				isValid = false;
			}
		}
		if (!isValid) {
			continue;
		}

		/*
		So, nun können wir
		- Den Block weglassen und die Folgezeilen das Z anpassen
		*/
		result.append(part, idx, a.start);
		idx = a.end + steps;
		double startZ = part[a.start].before().z;
        bool finalZreached = false;
		for (size_t i = 0; i < steps; ++i) {

			//Gehe doppelt so tief wie ursprünglich auf diesem Abschnitt, aber nicht tiefer als wir am Ende ein wollen
			double z = part[a.start + i].after().z;
			double newz = std::fmax(z + z - startZ, part[a.end - 1].after().z);
            
			//sicherstellen, dass wir am Ende den korrekten Wert haben ohne Rechenfehler
			if (i == steps - 1) {
				newz = part[a.end - 1].after().z;
			}
            
			Command cmd = part[a.end + i];
            if( !finalZreached ) cmd.setValue('Z', std::to_string(newz) );
            if( newz == part[a.end - 1].after().z ) finalZreached = true;
			result.append(cmd);
		}
	}
	result.append(part, idx);
	return result;
}

