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
#include "../include/cxxopts.hpp"
#include "common.h"

using json = nlohmann::json;

struct Params {
    int argc;
    enum class LogLevel {QUIET, VERBOSE, DEBUG, UNINITIALIZED} log_level = LogLevel::QUIET;
    std::string source;
    std::string logfile = "c-format-checker.log";
//    const std::string usage = "Usage: ./c-format-checker FILE [-q | -v | -d] [--logfile=FILE].\n"
//                              "Use «./c-format-checker --help» for explanation.";
    // Message similarly to the message for "grep" in GNU/Linux console
    const std::string usage = "Использование: ./c-format-checker [-f] ФАЙЛ [-q | -v | -d] [-l ФАЙЛ].\n"
                              "Запустите «./c-format-checker --help» для более подробного описания.\n";
    const std::string man =  // Аналогично русскояз. мануалу https://www.opennet.ru/man.shtml?topic=grep&category=1
        "НАЗВАНИЕ\n"
        "       c-format-checker - проверка файла исходных текстов программ на языке C  на корректность расстановки отступов.\n"
        "СИНТАКСИС\n"
        "       ./c-format-checker [-f] ФАЙЛ [-q | -v | -d] [-l ФАЙЛ]\n"
        "ОПИСАНИЕ\n"
        "       Программа сканирует файл и проверяет корректность расстановки отступов.\n"
        "       Если ошибки не обнаружены, программа завершается с кодом "
        + std::to_string(EXIT_CODE_OK) + ".\n"
        "       Если на одинаковом уровне вложенности строки имеют разный отступ, выдается сообщение об ошибке, "
        "программа завершается с кодом " + std::to_string(EXIT_CODE_INDENT_ERROR) +
        ".\n"
        "       Если в файле часть строк начинается с табуляций, а часть - с пробелов, выдается сообщение об ошибке, "
        "программа завершается с кодом " + std::to_string(EXIT_CODE_MIXED_SPACES) +
        ". (Отступы во всем файле должны отбиваться одинаковыми символами, "
        "комбинации пробелов и табуляций не допускаются.)\n"
        "       Если программе некорректно переданы аргументы при вызове, программа завершается с кодом "
        + std::to_string(EXIT_CODE_INVALID_ARG_ERROR) +
        ".\n"
        "       Если в программе произошла ошибка при попытке чтения/записи в файл, программа завершается с кодом "
        + std::to_string(EXIT_CODE_FILE_ACCESS_ERROR) +
        ".\n"
        "       Если произошла ошибка на этапе синтаксического разбора (несоответствие переданного файла грамматике), "
        "программа завершается с кодом " + std::to_string(EXIT_CODE_FILE_ACCESS_ERROR) +
        ".\n"
        "       Если во время выполнения программы произошла другая ошибка, программа завершается с кодом "
        + std::to_string(EXIT_CODE_INTERNAL_ERROR) +
        ".\n"
        ;


    Params(int argc, char *argv[]) :
    argc(argc)
    {
        cxxopts::Options options("c-format-checker",
                                 "проверка файла исходных текстов программ на языке C на корректность расстановки отступов");
        options.positional_help("[-f] ФАЙЛ [-q | -v | -d] [-l ФАЙЛ]");
        options
            .allow_unrecognised_options()
            .add_options()
                ("f,file",    "Файл исходного текста программы на языке c", cxxopts::value<std::string>())
                ("q,quiet",   "Выводить кратко описание ошибок (по умолчанию)")
                ("v,verbose", "Выводить развернутые описания ошибок", cxxopts::value<bool>()->default_value("false"))
                ("d,debug",   "Выводить отладочную информацию", cxxopts::value<bool>()->default_value("false"))
                ("help",      "Помощь", cxxopts::value<bool>()->default_value("false"))
                ("l,logfile", "Имя файла для записи отладочной информации", cxxopts::value<std::string>())
            ;
        options.parse_positional("file");
        auto result = options.parse(argc, argv);
        if (result.count("help")) {
            std::cout << man << std::endl;
            exit(EXIT_CODE_OK);
        }
        if (!result.count("file")) {
            std::cout << usage << std::endl;
            exit(EXIT_CODE_INVALID_ARG_ERROR);
        }
        source = result["file"].as<std::string>();
        if (result.count("quiet") + result.count("verbose") + result.count("debug") > 1) {
            std::cout << usage << std::endl;
            exit(EXIT_CODE_INVALID_ARG_ERROR);
        }
        if (result.count("quiet"))
            log_level = LogLevel::QUIET;
        if (result.count("verbose"))
            log_level = LogLevel::VERBOSE;
        if (result.count("debug"))
            log_level = LogLevel::DEBUG;
        if (result.count("logfile")) {
            logfile = result["logfile"].as<std::string>();
        } else {
            std::cout << usage << "\nФайл для вывода лога не указан, "
                                  "использован файл по умолчанию «" << logfile << "»." << std::endl;
        }
    }
};

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
    Params params(argc, argv);
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
    std::string mistakes = a.error_messages();
    if (mistakes.empty()) {
        return EXIT_CODE_OK;
    } else {
        return 1;
    }
}

