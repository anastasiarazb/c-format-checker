#include "frontend/lexer/coords.hpp"
#include "common.h"

void Coords::shift(int ch) {
    ++pos;
    if (ch != '\n') {
        ++ col;
    } else {
        col = 1;
        ++line;
    }
}

std::string Coords::to_string() const {
    return std::string("(") + std::to_string(line) + std::string(", ") + std::to_string(col)  +  std::string(")");
}

std::string Coords::coords_to_string(const Coords &start, const Coords &follow) {
    return start.to_string() + std::string("-") + follow.to_string();
}
