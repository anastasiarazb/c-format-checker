#ifndef COORDS_HPP
#define COORDS_HPP

#include <string>

class Coords
{
    int pos = 0, line = 1, col = 1;
public:
    Coords() = default;
    void shift(int ch);
    explicit operator std::string() const;
    static std::string coords_to_string(const Coords &start, const Coords &follow);
};


#endif // COORDS_HPP
