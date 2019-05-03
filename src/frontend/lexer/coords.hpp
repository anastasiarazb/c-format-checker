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
    static int distance(Coords start, Coords follow);
    int get_pos() const {return pos;}
    int get_line() const {return line;}
    int get_col() const {return col;}
};


#endif // COORDS_HPP
