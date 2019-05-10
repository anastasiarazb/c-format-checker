#ifndef C_FORMAT_CHECKER_LINE_HPP
#define C_FORMAT_CHECKER_LINE_HPP

#include "state.hpp"
#include "indent.hpp"
#include <string>


class Line : public std::vector<Token> {
    Indent indent;
public:
    Line(Indent indent);
    operator std::string() const;
};


#endif //C_FORMAT_CHECKER_LINE_HPP
