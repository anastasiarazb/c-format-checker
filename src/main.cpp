#include <iostream>
#include <fstream>
#include <typeinfo>
#include <vector>

#include "../include/nlohmann/json.hpp"
#include "frontend/lexer/scanner.hpp"
#include "frontend/rules/rules.h"

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

json read_rules(std::string path)
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
//    json j = read_rules("../rules.json");
//    std::vector<std::string> arr = j["case"];
//    std::cout << arr.size() << j["case"] << (j["case"].type() == json::value_t::array) << std::endl;
//    std::cout << (typeid(j["case"][0]).name() == typeid(char[]).name()) << std::endl;
    Rules r("../rules.json");
    std::cout << r << std::endl;
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

