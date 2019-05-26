#ifndef C_FORMAT_CHECKER_RULES_HPP
#define C_FORMAT_CHECKER_RULES_HPP

#include <unordered_map>
#include <unordered_set>

class Rules {

public:
    enum class Cases {
        BLOCK, CASE, CASE_STATEMENT, CORTEGE, ENUM, FUNCTION, IF_ELSE_WHILE_DO, LABEL, PRAGMA, STATEMENT, STRUCT, UNION,
        TERNARY_EXPRESSION // TERNARY_EXPRESSION is only for inner use; it should never be pushed to StateVector
    };

    enum class Indent {
        PLUS, ZERO, START, ANY
    };
    static const std::unordered_map<std::string, Cases> &str2case();
    static const std::unordered_map<std::string, Indent> &str2rule();
    static const std::unordered_map<Cases, std::unordered_set<Indent>> &case2rule_default();
    explicit Rules(const char *path);
    explicit operator std::string() const;
    std::unordered_set<Indent> &operator[](Cases usage);

private:
    std::unordered_map<Cases, std::unordered_set<Indent>> case2rule;

};

std::ostream &operator<<(std::ostream &os, Rules::Cases val);
std::ostream &operator<<(std::ostream &os, Rules::Indent val);
std::ostream &operator<<(std::ostream &os, const Rules &val);

std::string to_string(const Rules::Cases &obj);
std::string to_string(const Rules::Indent &obj);
std::string to_string(const std::unordered_set<Rules::Indent> &obj);


#endif //C_FORMAT_CHECKER_RULES_HPP
