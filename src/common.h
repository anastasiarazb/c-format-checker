#ifndef COMMON_H
#define COMMON_H

#include <string>

//#define DEBUG_LEXER
//#ifdef  DEBUG_LEXER
//#define log_l printf
//#else
//#define log_l(...) {}
//#endif

//enum {
//    EXIT_CODE_OK = 0,
//    EXIT_CODE_INDENT_ERROR = 1,
//    EXIT_CODE_MIXED_SPACES = 2,
//    EXIT_CODE_INVALID_ARG_ERROR = 3,
//    EXIT_CODE_FILE_ACCESS_ERROR = 4,
//    EXIT_CODE_PARSING_ERROR = 5,
//    EXIT_CODE_INTERNAL_ERROR = 6
//};


enum class LogLevel {
    QUIET, VERBOSE, DEBUG
};

#define EXIT_CODE_OK 0
#define EXIT_CODE_INVALID_ARG_ERROR 1
#define EXIT_CODE_FILE_ACCESS_ERROR 2
#define EXIT_CODE_PARSING_ERROR 3
#define EXIT_CODE_INDENT_ERROR 4
#define EXIT_CODE_MIXED_SPACES 5

//#define EXIT_CODE_INDENT_ERROR 1
//#define EXIT_CODE_MIXED_SPACES 2
//#define EXIT_CODE_INVALID_ARG_ERROR 3
//#define EXIT_CODE_FILE_ACCESS_ERROR 4
//#define EXIT_CODE_PARSING_ERROR 5
#define EXIT_CODE_INTERNAL_ERROR 6

std::string cannot_open_file_message(const std::string &path);

#endif //LEXER_H
