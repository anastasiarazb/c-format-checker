#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <vector>
#include <frontend/parser/parser.hpp>

#include "../include/nlohmann/json.hpp"
#include "frontend/lexer/scanner.hpp"
#include "frontend/rules/rules.hpp"
#include "ast/analyzer.hpp"
#include "ast/line.hpp"

using json = nlohmann::json;

void test_params(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Too less arguments!\n Usages: <program_source_file>\n. Please, try again!"
                  << std::endl;
        exit(1);
    }

    if (argc > 2) {
        std::cout << "Too much arguments, parameters ";
        for (int i = 2; i < argc - 1; ++i) {
            std::cout << "\"" << argv[i] << "\", ";
        }
        std::cout << "\"" << argv[argc - 1] << "\" ignored" << std::endl;
    }

}

json read_rules(const std::string &path)
{
    std::ifstream file (path, std::ios::in);
    if (!file) {
        std::cout << "No such file " << path << std::endl;
        exit(2);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
//    std::string text;
//    file.seekg(0, std::ios::end);
//    text.reserve(file.tellg());
//    file.seekg(0, std::ios::beg);
//    text.assign((std::istreambuf_iterator<char>(file)),
//                   std::istreambuf_iterator<char>());
    file.close();
    json j3 = json::parse(buffer);
    return j3;
}

int main(int argc, char* argv[])
{

    Rules r("../rules.json");
    std::cout << "Rules:\n" << r << std::endl;
    test_params(argc, argv);
    Scanner scan0 (argv[1]);
    Token token;
    Line st(Indent(Token(lexem::Type::NEWLINE, "\n", Coords(), Coords())));
    std::cout << "Tokens:\n" << std::endl;
    do {
        token = scan0.nextToken();
        st.push_back(token);
//        std::cout << "  " << "TOKEN " << token << std::endl;
    } while (token.type() != lexem::END_OF_FILE);
    scan0.print_errors();
//    st.set_rule(1, Rules::Cases::ENUM);
    std::cout << std::string(st) << std::endl;

    Scanner scanner(argv[1]);
    Parser parser(scanner);
    parser.parse();
    std::cout << parser.get_lines() << std::endl;
    std::cout << parser.get_errors_list() << std::endl;

    Analyzer a(parser.get_token_table(), r);
    return 0;
}

