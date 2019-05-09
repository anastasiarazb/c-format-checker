#ifndef INTERREPR_H
#define INTERREPR_H
#include <vector>
#include "state.hpp"

class InterRepr : public std::vector<State>
{
public:
    InterRepr();
};

#endif // INTERREPR_H
