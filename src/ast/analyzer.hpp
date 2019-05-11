#ifndef C_FORMAT_CHECKER_PARSETABLE_HPP
#define C_FORMAT_CHECKER_ANALYZER_HPP

#include "line.hpp"
#include <vector>
#include <string>

class Analyzer: public std::vector<Line> {
    Rules rules;
    std::unordered_map<Rules::Cases, std::pair<Indent, Token&>> stats;
public:
    explicit Analyzer(const std::vector<Line> &other, Rules rules);
    explicit operator std::string() const;
    void first_pass();
    void collect_stats();
    void analyze();

};


#endif //C_FORMAT_CHECKER_PARSETABLE_HPP
