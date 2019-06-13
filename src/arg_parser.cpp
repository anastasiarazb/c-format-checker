#include "arg_parser.hpp"
#include "../include/cxxopts.hpp"
#include "common.h"

ArgParser::ArgParser(int argc, char *argv[])
{
    cxxopts::Options options("c-format-checker",
                             "проверка файла исходных текстов программ на языке C на корректность расстановки отступов");
    options.positional_help("[-f] ФАЙЛЫ [-q | -v | -d]");
    options
//            .allow_unrecognised_options()
            .add_options()
                    ("f,file",    "Файл исходного текста программы на языке c", cxxopts::value<std::vector<std::string>>())
                    ("q,quiet",   "Выводить кратко описание ошибок (по умолчанию)")
                    ("v,verbose", "Выводить развернутые описания ошибок", cxxopts::value<bool>()->default_value("false"))
                    ("d,debug",   "Выводить отладочную информацию", cxxopts::value<bool>()->default_value("false"))
                    ("help",      "Помощь", cxxopts::value<bool>()->default_value("false"))
                    ("l,logfile", "Имя файла для записи отладочной информации", cxxopts::value<std::string>())
            ;
    options.parse_positional("file");
    try {
        auto result = options.parse(argc, argv);
        if (result.count("help")) {
            std::cout << man << std::endl;
            exit(EXIT_CODE_OK);
        }
        if (!result.count("file")) {
            std::cout << usage << std::endl;
            exit(EXIT_CODE_INVALID_ARG_ERROR);
        }
        sources = result["file"].as<std::vector<std::string>>();
        source_file = sources.cbegin();
        if (result.count("quiet") + result.count("verbose") + result.count("debug") > 1) {
            std::cout << usage << std::endl;
            exit(EXIT_CODE_INVALID_ARG_ERROR);
        }
        if (result.count("quiet"))
            log_level = LogLevel::QUIET;
        else if (result.count("verbose"))
            log_level = LogLevel::VERBOSE;
        else if (result.count("debug"))
            log_level = LogLevel::DEBUG;
    } catch(...) {
        std::cout << man << std::endl;
        exit(EXIT_CODE_INVALID_ARG_ERROR);
    }
//    if (result.count("logfile")) {
//        log_path = result["logfile"].as<std::string>();
//        logs = Logger(log_level, log_path);
//    }
//    else {
//        std::cout << usage << "\nФайл для вывода лога не указан, "
//                              "использован файл по умолчанию «" << log_path << "»." << std::endl;
//    }
//    logs = std::ofstream(log_path);
//    if (!logs) {
//        std::cout << cannot_open_file_message(log_path) << std::endl;
//        exit(EXIT_CODE_FILE_ACCESS_ERROR);
//    }
}

ArgParser::~ArgParser()
{
//    if (logs && logs.is_open()) {
//        logs.close();
//    }
}

bool ArgParser::hasNextParams() const
{
    return source_file != sources.cend();
}

Params ArgParser::nextParams()
{
    if (source_file == sources.cend()) {
        throw std::logic_error("Reach the end of the source files vector");
    }
    auto file_it = source_file;
    ++source_file;
    return Params(*file_it, log_level);
}

