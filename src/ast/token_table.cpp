#include "token_table.hpp"


void TokenTable::pushToken(const Token &token)
{
    back().push_back(token);
}

void TokenTable::addLine(Indent indent, const StateVector &rule_cases)
{
    if (back().empty()) {  // Store only non_empty lines
        pop_back();
    }
    emplace_back(indent);
    back().addState(rule_cases);
}

Token &TokenTable::lastToken()
{
    return back().back();
}

void TokenTable::correctState(const StateVector &rule_cases)
{
    if (back().size() == 1) {
        // We correct state for the first token in line => correct state for the while line
        back().resetState(rule_cases);
    }  // else: do nothing
}

Line &TokenTable::getLine(int num_in_src, int *num_in_vector)
{
    if (back().number() < num_in_src) {
        throw std::range_error(std::string(__FUNCTION__) + ": index " + std::to_string(num_in_src)
                               + " out of range (max= " + std::to_string(back().number()) + ").");
    }
    for(int i = 0; (size_t)i < size(); ++i) {
        Line &line = at(i);
        if (line.number() == num_in_src) {
            if (num_in_vector != nullptr) {
                *num_in_vector = i;
            }
            return line;
        }
    }
    throw std::range_error(std::string(__FUNCTION__) + ": index " + std::to_string(num_in_src) + " not found.");
}


int TokenTable::vectorIndex(int source_index)
{
    if (back().number() < source_index) {
        throw std::range_error(std::string(__FUNCTION__) + ": index " + std::to_string(source_index)
                               + " out of range (max= " + std::to_string(back().number()) + ").");

    }
    for(size_t i = 0; i < size(); ++i) {
        if (at(i).number() == source_index) {
            return i;
        }
    }
    throw std::range_error(std::string(__FUNCTION__) + ": index " + std::to_string(source_index) + " not found.");
//    return  -1;
}