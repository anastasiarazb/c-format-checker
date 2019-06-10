#ifndef C_FORMAT_CHECKER_RULES_HPP
#define C_FORMAT_CHECKER_RULES_HPP

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

class Rules {

public:

    enum class Cases {
        BLOCK, SWITCH, CASE, CASE_STATEMENT, CASE_BLOCK, CORTEGE, FUNCTION, IF_ELSE_WHILE_DO, LABEL, PRAGMA, STATEMENT,
        STRUCT,  ENUM, UNION,
        TERNARY_EXPRESSION // TERNARY_EXPRESSION is only for inner use; it should never be pushed to StateVector
    };

    enum class Indent {
        ANY, PLUS, // ZERO, START
    };
    const Indent DEFAULT_INDENT = Indent::PLUS;
    static const std::unordered_map<std::string, Cases> &str2case();
    static const std::unordered_map<std::string, Indent> &str2rule();
    static const std::unordered_map<Cases, std::unordered_set<Indent>> &case2rule_default();
    explicit Rules(const std::string &path);
    explicit operator std::string() const;
    std::unordered_set<Indent> &operator[](Cases usage);
    const std::string path() {return m_path;}

private:
    std::unordered_map<Cases, std::unordered_set<Indent>> case2rule;
    std::string m_path;
    std::unordered_set<Rules::Indent> from_vec(const std::vector<std::string> &values);

};

std::ostream &operator<<(std::ostream &os, Rules::Cases val);
std::ostream &operator<<(std::ostream &os, Rules::Indent val);
std::ostream &operator<<(std::ostream &os, const Rules &val);

std::string to_string(const Rules::Cases &obj);
std::string to_string(const Rules::Indent &obj);
std::string to_string(const std::unordered_set<Rules::Indent> &obj);


#endif //C_FORMAT_CHECKER_RULES_HPP
