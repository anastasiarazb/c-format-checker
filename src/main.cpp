#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <vector>

#include "frontend/parser/parser.hpp"
#include "frontend/lexer/scanner.hpp"
#include "frontend/lexer/token.hpp"
#include "frontend/rules/rules.hpp"
#include "ast/analyzer.hpp"
#include "ast/line.hpp"
#include "../include/nlohmann/json.hpp"
#include "common.h"
#include "arg_parser.hpp"
#include "params.hpp"


using json = nlohmann::json;

std::string cannot_open_file_message(const std::string &path)
{
    return "Невозможно открыть файл: " + path;
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

#define LOG(...) \
do { \
    /*logfile <<  __VA_ARGS__ << std::endl;*/ \
    if (params.log_level == LogLevel::DEBUG) std::cout << __VA_ARGS__ << std::endl; \
} while(0)

int process_file(const Params &params, const Rules &rules)
{
    if (params.log_level == LogLevel::DEBUG) {
        Scanner debug_scanner(params);
        Token token;
        Line st(Indent(Token(lexem::Type::NEWLINE, "\n", Coords(), Coords())));
        LOG("Tokens:\n");
        do {
            token = debug_scanner.nextToken();
            st.push_back(token);
        } while (token.type() != lexem::END_OF_FILE);
        LOG(std::string(st));
    }

    Scanner scanner(params);
    std::string scanner_errors = scanner.error_messages();
    if (!scanner_errors.empty()) {
//        logfile   << params.message_header() << " Ошибка на этапе лексического анализа: " << scanner_errors << std::endl;
        std::cout << params.message_header() << " Ошибка на этапе лексического анализа: " << scanner_errors << std::endl;
        return EXIT_CODE_PARSING_ERROR;
    }
    Parser parser(scanner, params);
    parser.parse();
    LOG(parser.get_lines());
    std::string parser_errors = parser.error_messages();
    if (!parser_errors.empty()) {
//        logfile   << params.message_header() << " Ошибка на этапе синтаксического анализа: " << parser_errors << std::endl;
        std::cout << params.message_header() << " Ошибка на этапе синтаксического анализа: " << parser_errors << std::endl;
        return EXIT_CODE_PARSING_ERROR;
    }

    Analyzer analyzer(parser.get_token_table(), rules, params);
    analyzer.analyze();
    std::string mistakes = analyzer.error_messages();
    if (!mistakes.empty()) {
//    logfile   << mistakes << std::endl;
        std::cout << mistakes << std::endl;
    }
    return analyzer.get_exit_code();
}


int main(int argc, char* argv[])
{
    ArgParser arg_parser(argc, argv);
//    std::ofstream logfile(params.log_path);
//    if (!logfile) {
//        std::cout << cannot_open_file_message(params.log_path) << std::endl;
//        exit(EXIT_CODE_FILE_ACCESS_ERROR);
//    }
//    logfile << "Анализ программы " << params.source << std::endl;
    Rules rules("rules.json");
    if (arg_parser.log_level == LogLevel::DEBUG) {
        std::cout << "Rules:\n" << rules << std::endl;
    }
    std::ofstream statfile("cfc-statistics.txt");
    if (!statfile) {
        std::cout << cannot_open_file_message("cfc-statistics.txt") << std::endl;
        exit(EXIT_CODE_FILE_ACCESS_ERROR);
    }
    while(arg_parser.hasNextParams()) {
        Params params = arg_parser.nextParams();
        int exit_code = process_file(params, rules);
        switch (exit_code) {
            case EXIT_CODE_MIXED_SPACES:
                statfile << params.source << "   (tab.)" << std::endl;
                break;
            case EXIT_CODE_INDENT_ERROR:
                statfile << params.source << std::endl;
                break;
            default:
                break;
        }
    }
    statfile.close();

    return 0;
}

