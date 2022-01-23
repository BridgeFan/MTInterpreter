//
// Created by kamil-hp on 18.11.2021.
//

#include <cmath>
#include "Number.h"
#include "../ErrorHandler.h"
#include "Util.h"

Number::Number(int line, int column, int64_t val) : Token(Number_, line, column), value(val) {
}

Number::Number(int line, int column, double val) : Token(Number_, line, column), value(val) {
}