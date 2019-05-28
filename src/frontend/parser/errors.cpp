#include <sstream>
#include "parser.hpp"

std::string Parser::error_messages() const
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
    std::string coords(token.coords_to_string() + " ");
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
        nextTokenPragma();
    }
    return token;
}

void Parser::write_message(const std::string &message, char const *file, int line)
{
    std::stringstream ss;
    ss << std::string("[") << file << ", line " << line << "] " << message << ".";
    errors_list.push_back(ss.str());
    logs << ss.str() << std::endl;
    if (params.log_level == Params::LogLevel::DEBUG) {
        std::cout << ss.str() << std::endl;
    }
}

const Token &Parser::process_error(const std::vector<lexem::Type> &expected,
                                   const std::vector<lexem::Type> &skip_until,
                                   char const *file, int line)
{
    std::stringstream message;
    message << "Error at " << token.coords_to_string() << " - expected ";
    std::string delimiter("|");
    for (const lexem::Type &lexem: expected) {
        message << to_string(lexem) << delimiter;
    }
    message.seekp(-1, std::ios_base::end);
    message << ", got " << to_string(token.type());

    write_message(message.str(), file, line);

    while (!token.in(skip_until) && token.type() != lexem::END_OF_FILE) {
//        nextToken(RETURN_NEWLINES);
        nextTokenPragma();
    }
    return token;
}

const Token &Parser::checkToken(const std::vector<lexem::Type> &expected,
                        const std::vector<lexem::Type> &skip_until,
                        char const *file, int line)
{
    if (token.in(expected)) {
        return token;
    } else {
        process_error(expected, skip_until, file, line);
        return token;
    }
}
