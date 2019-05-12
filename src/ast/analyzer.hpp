#ifndef C_FORMAT_CHECKER_PARSETABLE_HPP
#define C_FORMAT_CHECKER_ANALYZER_HPP

#include "line.hpp"
#include "state_vector.hpp"
#include <vector>
#include <string>
#include <map>

class Analyzer: public std::vector<Line> {
    Rules rules;
    std::multimap<StateVector, Indent> stats;
public:
    std::string str_stats() const;
    explicit Analyzer(const std::vector<Line> &other, Rules rules);
    explicit operator std::string() const;
    void first_pass();
    void collect_stats();
    void analyze();

};


#endif //C_FORMAT_CHECKER_PARSETABLE_HPP
