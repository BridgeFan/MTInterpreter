//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_BLOCKNODE_H
#define MTINTERPRETER_BLOCKNODE_H

#include <vector>
#include "Line.h"
#include <variant>
#include <any>


struct Block: public Line {
	std::vector<std::reference_wrapper<Line> > lines;
};

#endif //MTINTERPRETER_BLOCKNODE_H
