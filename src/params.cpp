#include <sstream>
#include "params.hpp"
#include "common.h"

//Logger::Logger(LogLevel log_level, const std::string &path):
//log_level(log_level)
//{
//    if (path.empty()) {
//        return;
//    }
//    logs = std::ofstream(path);
//    if (!logs) {
//        std::cout << cannot_open_file_message(path) << std::endl;
//        exit(EXIT_CODE_FILE_ACCESS_ERROR);
//    }
//
//}

//Logger &operator << (Logger &out, std::basic_ostream<char> &(*f)(std::basic_ostream<char> &))
//{
//    if (out.logs && out.logs.is_open()) {
//        out.logs << f;
//        return out;
//    }
//    return out;
//}
//
//Logger::~Logger()
//{
//    if (logs && logs.is_open()) {
//        logs.close();
//    }
//}

Params::Params(std::string source, LogLevel log_level) :
        source(std::move(source)), log_level(log_level)
{

}


Params::~Params()
{
//    if (logs && logs.is_open()) {
//        logs.close();
//    }
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