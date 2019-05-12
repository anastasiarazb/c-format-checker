#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <sstream>
#include <unordered_set>
#include "rules.hpp"

using json = nlohmann::json;

const std::unordered_map<std::string, Rules::Cases> &Rules::str2case() {
    static const std::unordered_map<std::string, Rules::Cases> cases
        {
            {"statement",        Rules::Cases::STATEMENT},
            {"struct",           Rules::Cases::STRUCT},
            {"enum",             Rules::Cases::ENUM},
            {"union",            Rules::Cases::UNION},
            {"function",         Rules::Cases::FUNCTION},
            {"if-else-while-do", Rules::Cases::IF_ELSE_WHILE_DO},
            {"label",            Rules::Cases::LABEL},
            {"case",             Rules::Cases::CASE},
            {"case_statement",   Rules::Cases::CASE_STATEMENT},
            {"block",            Rules::Cases::BLOCK},
            {"pragma",           Rules::Cases::PRAGMA},
            {"cortege",          Rules::Cases::CORTEGE}
        };
    return cases;
}

const std::unordered_map<std::string, Rules::Indent> &Rules::str2rule() {
    static const std::unordered_map<std::string, Rules::Indent> rules {
        {"+",     Rules::Indent::PLUS},
        {"0",     Rules::Indent::ZERO},
        {"start", Rules::Indent::START},
        {"any",   Rules::Indent::ANY}
    };
    return rules;
}

const std::unordered_map<Rules::Cases, std::unordered_set<Rules::Indent>> &Rules::case2rule_default() {
    static const std::unordered_map<Rules::Cases, std::unordered_set<Rules::Indent>> rules_default{
        {Rules::Cases::STATEMENT,        {Rules::Indent::ANY}},
        {Rules::Cases::STRUCT,           {Rules::Indent::PLUS}},
        {Rules::Cases::ENUM,             {Rules::Indent::PLUS}},
        {Rules::Cases::UNION,            {Rules::Indent::PLUS}},
        {Rules::Cases::FUNCTION,         {Rules::Indent::PLUS}},
        {Rules::Cases::IF_ELSE_WHILE_DO, {Rules::Indent::PLUS}},
        {Rules::Cases::LABEL,            {Rules::Indent::START, Rules::Indent::ZERO}},
        {Rules::Cases::CASE,             {Rules::Indent::ZERO,  Rules::Indent::PLUS}},
        {Rules::Cases::CASE_STATEMENT,   {Rules::Indent::PLUS}},
        {Rules::Cases::BLOCK,            {Rules::Indent::PLUS}},
        {Rules::Cases::PRAGMA,           {Rules::Indent::START}},
        {Rules::Cases::CORTEGE,          {Rules::Indent::ANY}},
    };
    return rules_default;
}

json read_to_json(const char *path)
{
    std::ifstream file (path, std::ios::in);
    if (!file) {
        std::cout << "No such file " << path << std::endl;
        exit(2);
    }
    std::string text;
    file.seekg(0, std::ios::end);
    text.reserve(static_cast<unsigned int>(file.tellg()));
    file.seekg(0, std::ios::beg);
    text.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
    file.close();
    json j = json::parse(text);
    return j;
}

std::unordered_set<Rules::Indent> from_vec(const std::vector<std::string> &values) {
    std::unordered_set<Rules::Indent> res {};
    for (const std::string &val: values) {
        res.insert(Rules::str2rule().at(val));
    }
    return res;
}

Rules::Rules(const char *path)
{
    json j = read_to_json(path);
    for (const auto &[string_repr, case_const]: str2case()) {
        if (j.find(string_repr) != j.end()) {
            case2rule[case_const] = from_vec(j[string_repr]);
        } else {
            case2rule[case_const] = case2rule_default().at(case_const);
            std::cout << "[Rules::Rules]: name for constant " << to_string(case_const)
            << " not found, used default value " << to_string(case2rule[case_const]);
        }
    }

    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        if (str2case().find(it.key()) == str2case().end()) {
            std::cout << "[Rules::Rules]: key " << it.key() << "from file " << path << " not found. Ignored.";
        }
    }
//    std::cout << std::to_string(case2rule[Cases::CASE]) << std::endl;

}

std::unordered_set<Rules::Indent> &Rules::operator[](Rules::Cases usage)
{
    return case2rule[usage];
}

std::string to_string(const Rules::Cases &obj)
{
    static std::unordered_map<Rules::Cases, std::string> represents {
        {Rules::Cases::STATEMENT,        "STATEMENT"},
        {Rules::Cases::STRUCT,           "STRUCT"},
        {Rules::Cases::ENUM,             "ENUM"},
        {Rules::Cases::UNION,            "UNION"},
        {Rules::Cases::FUNCTION,         "FUNCTION"},
        {Rules::Cases::IF_ELSE_WHILE_DO, "IF_ELSE_WHILE_DO"},
        {Rules::Cases::LABEL,            "LABEL"},
        {Rules::Cases::CASE,             "CASE"},
        {Rules::Cases::CASE_STATEMENT,   "CASE_STATEMENT"},
        {Rules::Cases::BLOCK,            "BLOCK"},
        {Rules::Cases::PRAGMA,           "PRAGMA"},
        {Rules::Cases::CORTEGE,          "CORTEGE"},
    };
    return represents.at(obj);
}

std::string to_string(const std::unordered_set<Rules::Indent> &obj)
{
    std::stringstream res;
    res << "{";
    for (const Rules::Indent &rule: obj) {
        res << to_string(rule) << "|";
    }
//    res.seekg(-1,std::ios::end);
    res.seekp(static_cast<unsigned int>(res.tellp())-1);
    res << "}";
    return res.str();
}

std::string to_string(const Rules::Indent &obj)
{
    static std::unordered_map<Rules::Indent, std::string> represents {
        {Rules::Indent::PLUS, "PLUS"},
        {Rules::Indent::ZERO, "ZERO"},
        {Rules::Indent::START, "START"},
        {Rules::Indent::ANY, "ANY"},

    };
    return represents.at(obj);
}

std::ostream &operator<<(std::ostream &os, Rules::Cases val)
{
    return os << to_string(val);
}
std::ostream &operator<<(std::ostream &os, Rules::Indent val)
{
    return os << to_string(val);
}

std::ostream &operator<<(std::ostream &os, const Rules &val)
{
    return os << std::string(val);
}

Rules::operator std::string() const
{
    std::stringstream res;
    for (const auto &[case_const, rules] : case2rule) {
        res << case_const << ":\n   ";
        res << to_string(rules);
//        for (const Rule &rule: rules) {
//            res << to_string(rule) << " | ";
//        }
        res << '\n';
    }
    return res.str();
}
