#include "params.hpp"
#include "../include/cxxopts.hpp"
#include "common.h"

Params::Params(int argc, char *argv[]) :
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
        log_path = result["logfile"].as<std::string>();
    } else {
        std::cout << usage << "\nФайл для вывода лога не указан, "
                              "использован файл по умолчанию «" << log_path << "»." << std::endl;
    }
    logs = std::ofstream(log_path);
    if (!logs) {
        std::cout << cannot_open_file_message(log_path) << std::endl;
        exit(EXIT_CODE_FILE_ACCESS_ERROR);
    }
}

Params::~Params()
{
    if (logs && logs.is_open()) {
        logs.close();
    }
}

std::string Params::message_header(int line_number) const
{
    std::stringstream ss;
    ss << source;
    if (line_number > 0) {
        ss << " (строка " << line_number << ")";
    }
    ss << ":";
    return ss.str();
}