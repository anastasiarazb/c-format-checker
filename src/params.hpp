#ifndef C_FORMAT_CHECKER_PARAMS_HPP
#define C_FORMAT_CHECKER_PARAMS_HPP

#include "common.h"
#include <iostream>
#include <sstream>
#include <sstream>

//
//struct Logger : std::stringstream {
//    std::stringstream logs;
//    LogLevel log_level;
//    Logger() = default;
////    Logger(Logger &&) = default;
//    explicit Logger(LogLevel log_level, const std::string &path = "");
//    ~Logger();
//};

//
//template<typename T>
//Logger &operator << (Logger &out, const T &obj)
//{
//    if (out.logs && out.logs.is_open()) {
//        out.logs << obj;
//        return out;
//    }
//    return out;
//}
//
//
//Logger &operator << (Logger &out, std::basic_ostream<char> &(*f)(std::basic_ostream<char> &));


struct Params {
    const std::string source;
    const LogLevel log_level;
    Params(std::string source, LogLevel log_level);
    ~Params();

    std::string message_header(int line_number = -1) const;
};

#endif //C_FORMAT_CHECKER_PARAMS_HPP
