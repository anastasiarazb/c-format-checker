#ifndef C_FORMAT_CHECKER_PARAMS_HPP
#define C_FORMAT_CHECKER_PARAMS_HPP

#include "common.h"
#include <iostream>
#include <fstream>

struct Params {
    int argc;
    enum class LogLevel {QUIET, VERBOSE, DEBUG} log_level = LogLevel::QUIET;
    std::string source;
    std::string log_path = "c-format-checker.log";
    std::ofstream logs;
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
        + std::to_string(EXIT_CODE_OK) +
        ".\n"
        "       Если на одинаковом уровне вложенности строки имеют разный отступ, выдается сообщение об ошибке, "
        "программа завершается с кодом " + std::to_string(EXIT_CODE_INDENT_ERROR) +
        ".\n"
        "       Если в файле часть строк начинается с табуляций, а часть - с пробелов, выдается сообщение об ошибке, "
        "программа завершается с кодом " + std::to_string(EXIT_CODE_MIXED_SPACES) +
        ". (Отступы во всем файле должны отбиваться одинаковыми символами, "
        "комбинации пробелов и табуляций не допускаются.)\n"

        "       Если произошла ошибка на этапе лексического или синтаксического разбора из-за несоответствия исходного "
        "текста грамматике, программа завершается с кодом " + std::to_string(EXIT_CODE_PARSING_ERROR) +
        ".\n"
        "ОПЦИИ\n"
        "       -f, --file    Файл исходного текста программы на языке С (ключ -f не требуется, если этот аргумент идет первым).\n"
        "       -q, --quiet   Выводить кратко описание ошибок (по умолчанию).\n"
        "       -v, --verbose Выводить развернутые описания ошибок.\n"
        "       -d, --debug   Выводить отладочную информацию.\n"
        "       --help        Помощь.\n"
        "       -l, --logfile Имя файла для записи отладочной информации (по умолчанию: " + log_path + ").\n"
        "СТАТУС ВЫХОДА\n       "
        + std::to_string(EXIT_CODE_OK) + "   Ошибки не обнаружены.\n       "
        + std::to_string(EXIT_CODE_INDENT_ERROR) + "   На одинаковом уровне вложенности строки имеют разный отступ.\n       "
        + std::to_string(EXIT_CODE_MIXED_SPACES) + "   В файле часть строк начинается с табуляций, а часть - с пробелов.\n       "
        + std::to_string(EXIT_CODE_INVALID_ARG_ERROR) + "   Некорректные аргументы командной строки.\n       "
        + std::to_string(EXIT_CODE_FILE_ACCESS_ERROR) + "   Ошибка чтения/записи в файл.\n       "
        + std::to_string(EXIT_CODE_PARSING_ERROR)     + "   Произошла ошибка на этапе лексического или синтаксического"
                                                        " разбора из-за несоответствия исходного текста грамматике.\n"
    ;
    Params(int argc, char* argv[]);
    ~Params();
};

#endif //C_FORMAT_CHECKER_PARAMS_HPP
