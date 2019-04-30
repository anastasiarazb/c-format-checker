#include <iostream>
#include <fstream>
#include <typeinfo>

#include "../include/nlohmann/json.hpp"
#include "frontend/lexer/scanner.hpp"

using json = nlohmann::json;

void test_params(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Too less arguments!\n Usages: <progam_source_file>\n. Please, try again!"
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

json parse_rules(std::string path)
{
    std::ifstream file (path, std::ios::in);
    if (!file) {
        std::cout << "No such file " << path << std::endl;
        exit(2);
    }
    std::string text;
    file.seekg(0, std::ios::end);
    text.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    text.assign((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
    file.close();
    json j3 = json::parse(text);
    return j3;
}

int main(int argc, char* argv[])
{
    json j = parse_rules("../rules.json");
    std::cout << j["case"][1] << '\n'
        << typeid(j["case"][1]).name() << '\n'
        << typeid(j["case"]).name() << '\n'
        << typeid(char *).name() << '\n' << typeid(char []).name() << std::endl;
    std::cout << (typeid(j["case"][0]).name() == typeid(char[]).name()) << std::endl;
//    test_params(argc, argv);
//    Scanner scan (argv[1]);
//    Token token;
//    do {
//        token = scan.nextToken();
//        std::cout << "  " << "TOKEN " << token << std::endl;
//    } while (token.type() != lexem::END_OF_FILE);
//    scan.print_errors();
//    return 0;
}

