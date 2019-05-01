#ifndef C_FORMAT_CHECKER_RULES_HPP
#define C_FORMAT_CHECKER_RULES_HPP

#include <unordered_map>
#include <unordered_set>

class Rules {

public:
    enum class Cases {
        STRUCT, ENUM, UNION, FUNCTION, IF_ELSE_WHILE_DO, LABEL, CASE, CASE_STATEMENT, BLOCK, PRAGMA, CORTEGE
    };

    enum class Rule {
        PLUS, ZERO, START, ANY
    };
    static const std::unordered_map<std::string, Cases> &str2case();
    static const std::unordered_map<std::string, Rule> &str2rule();
    static const std::unordered_map<Cases, std::unordered_set<Rule>> &case2rule_default();
    explicit Rules(const char *path);
    explicit operator std::string() const;

private:
    std::unordered_map<Cases, std::unordered_set<Rule>> case2rule;

};

std::ostream &operator<<(std::ostream &os, Rules::Cases val);
std::ostream &operator<<(std::ostream &os, Rules::Rule val);
std::ostream &operator<<(std::ostream &os, const Rules &val);

std::string to_string(const Rules::Cases &obj);
std::string to_string(const Rules::Rule &obj);
std::string to_string(const std::unordered_set<Rules::Rule> &obj);


#endif //C_FORMAT_CHECKER_RULES_HPP
