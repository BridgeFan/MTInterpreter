//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_BLOCKNODE_H
#define MTINTERPRETER_BLOCKNODE_H

#include <vector>
#include "Line.h"
#include <variant>
#include <any>
#include <memory>


struct Block: public Line {
	std::vector<std::shared_ptr<Line> > lines;
	void accept(Visitor& visitor) override;
};

#endif //MTINTERPRETER_BLOCKNODE_H
