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

Coords::operator std::string() const {
    return std::string("(") + std::to_string(line) + std::string(", ") + std::to_string(col)  +  std::string(")");
}

std::string Coords::coords_to_string(const Coords &start, const Coords &follow) {
    return std::string(start) + std::string("-") + std::string(follow);
}

int Coords::distance(Coords start, Coords follow)
{
    return follow.pos - start.pos;
}