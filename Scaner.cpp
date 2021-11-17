//
// Created by kamil-hp on 17.11.2021.
//

#include "Scaner.h"

std::shared_ptr<Token> Scaner::getNextToken() {
	return {};
}

Scaner::Scaner(std::unique_ptr<DataSource> &&source) : source(std::move(source)) {}

bool Scaner::hasEnded() const {
	return source->hasEnded();
}
