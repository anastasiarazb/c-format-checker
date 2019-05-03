#include <sstream>
#include "parser.hpp"

std::string Parser::get_errors_list()
{
    std::stringstream ss;
//    scanner.print_errors();
    for (const auto & message: errors_list) {
        ss << message << "\n";
    }
    return ss.str();
}

const Token &Parser::process_error(lexem::Type expected, lexem::Type skip_until) {
    std::string error_massage("Error at ");
    std::string coords(token.coords_to_string());
    std::string tok_of_type(" - token of type ");
    std::string expected_type(to_string(expected));
    std::string got(" expected, got ");
    std::string got_type(to_string(token.type()));
    std::string l_arrow(" <");
    std::string r_arrow(">.");
    std::string got_image(token.image());

    size_t sum_size = error_massage.size() +  coords.size() + tok_of_type.size() + expected_type.size()
                      + got.size() + got_type.size() + l_arrow.size() + got_image.size() + r_arrow.size() + got_image.size();
    error_massage.reserve(sum_size);
    error_massage.append(coords).append(tok_of_type).append(expected_type).append(got).append(got_type)
        .append(l_arrow).append(got_image).append(r_arrow);

    errors_list.push_back(error_massage);
    while (token.type() != skip_until && token.type() != lexem::END_OF_FILE) {
        nextToken();
    }
    return token;
}

const Token &Parser::process_error(lexem::Type expected,
                                   const std::vector<lexem::Type> &skip_until) {
    std::string error_message("Error at ");
    std::string coords(token.coords_to_string());
    std::string tok_of_type(" - token of type ");
    std::string expected_type(to_string(expected));
    std::string got(" expected, got ");
    std::string got_type(to_string(token.type()));
    std::string l_arrow(" <");
    std::string r_arrow(">.");
    std::string got_image(token.image());

    size_t sum_size = error_message.size() +  coords.size() + tok_of_type.size() + expected_type.size()
                      + got.size() + got_type.size() + l_arrow.size() + got_image.size() + r_arrow.size() + got_image.size();
    error_message.reserve(sum_size);
    error_message.append(coords).append(tok_of_type).append(expected_type).append(got).append(got_type)
        .append(l_arrow).append(got_image).append(r_arrow);

    errors_list.push_back(error_message);
    while (!token.in(skip_until) && token.type() != lexem::Type::END_OF_FILE) {
        nextToken();
    }
    return token;
}

const Token &Parser::process_error(lexem::Type expected, lexem::Type skip_until
    , char const *file, int line)
{
    std::string error_massage("Error at ");
    std::string coords(token.coords_to_string());
    std::string file_str(file);
    std::string line_str(std::to_string(line));
    std::string ws(" ");
    std::string tok_of_type(" - token of type ");
    std::string expected_type(to_string(expected));
    std::string got(" expected, got ");
    std::string got_type(to_string(token.type()));
    std::string l_arrow(" <");
    std::string r_arrow(">.");
    std::string got_image(token.image());

    size_t sum_size = error_massage.size()
                      + file_str.size() + ws.size() + line_str.size() + ws.size() + coords.size()
                      + tok_of_type.size() + expected_type.size()
                      + got.size() + got_type.size()
                      + l_arrow.size() + got_image.size() + r_arrow.size()
                      + got_image.size();
    error_massage.reserve(sum_size);
    error_massage.append(coords)
        .append(file_str).append(ws).append(line_str).append(ws)
        .append(tok_of_type).append(expected_type)
        .append(got).append(got_type)
        .append(l_arrow).append(got_image).append(r_arrow);

    errors_list.push_back(error_massage);
    while (token.type() != skip_until && token.type() != lexem::END_OF_FILE) {
        nextToken();
    }
    return token;
}

const Token &Parser::process_error(lexem::Type expected,
                                   const std::vector<lexem::Type> &skip_until,
                                   char const *file, int line)
{
    std::string error_massage("Error at ");
    std::string coords(token.coords_to_string());
    std::string file_str(file);
    std::string line_str(std::to_string(line));
    std::string ws(" ");
    std::string tok_of_type(" - token of type ");
    std::string expected_type(to_string(expected));
    std::string got(" expected, got ");
    std::string got_type(to_string(token.type()));
    std::string l_arrow(" <");
    std::string r_arrow(">.");
    std::string got_image(token.image());

    size_t sum_size = error_massage.size()
                      + file_str.size() + ws.size() + line_str.size() + ws.size() + coords.size()
                      + tok_of_type.size() + expected_type.size()
                      + got.size() + got_type.size()
                      + l_arrow.size() + got_image.size() + r_arrow.size()
                      + got_image.size();
    error_massage.reserve(sum_size);
    error_massage.append(coords)
        .append(file_str).append(ws).append(line_str).append(ws)
        .append(tok_of_type).append(expected_type)
        .append(got).append(got_type)
        .append(l_arrow).append(got_image).append(r_arrow);

    errors_list.push_back(error_massage);
    while (!token.in(skip_until) && token.type() != lexem::END_OF_FILE) {
        nextToken();
    }
    return token;
}

const Token &Parser::process_error(const std::vector<lexem::Type> &expected,
                                   const std::vector<lexem::Type> &skip_until,
                                   char const *file, int line)
{
    std::string error_massage("Error at ");
    std::string coords(token.coords_to_string());
    std::string file_str(file);
    std::string line_str(std::to_string(line));
    std::string ws(" ");
    std::string tok_of_type(" - tokens of types ");
    std::string expected_type;
    std::string delimiter("|");
    for (const lexem::Type &lexem: expected) {
        expected_type.append(to_string(lexem)).append(delimiter);
    }
    expected_type.erase(expected_type.size()-delimiter.size());
    std::string got(" expected, got ");
    std::string got_type(to_string(token.type()));
    std::string l_arrow(" <");
    std::string r_arrow(">.");
    std::string got_image(token.image());

    size_t sum_size = error_massage.size()
                      + file_str.size() + ws.size() + line_str.size() + ws.size() + coords.size()
                      + tok_of_type.size() + expected_type.size()
                      + got.size() + got_type.size()
                      + l_arrow.size() + got_image.size() + r_arrow.size()
                      + got_image.size();
    error_massage.reserve(sum_size);
    error_massage.append(coords)
        .append(file_str).append(ws).append(line_str).append(ws)
        .append(tok_of_type).append(expected_type)
        .append(got).append(got_type)
        .append(l_arrow).append(got_image).append(r_arrow);

    errors_list.push_back(error_massage);

    while (!token.in(skip_until) && token.type() != lexem::END_OF_FILE) {
        nextToken();
    }
    return token;
}
