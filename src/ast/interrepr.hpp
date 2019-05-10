#ifndef INTERREPR_H
#define INTERREPR_H
#include <vector>
#include "state.hpp"
#include "line.hpp"

class InterRepr : public std::vector<Line>
{
public:
    InterRepr();
};

#endif // INTERREPR_H
