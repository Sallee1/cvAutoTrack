#pragma once
#include <string>
#include <list>
#include <memory>
#include <chrono>
#if cplusplus >= 202002L
#include <source_location>
#endif

#include <opencv2/opencv.hpp>
#include <Windows.h>

namespace tianli::global
{
    class error_type
    {
    public:
        int code;
        std::string msg;
#if cplusplus >= 202002L
        std::source_location location;
        #else
        std::string location;
#endif
    };

    class error_info
    {
        public:
            std::chrono::system_clock::time_point time;
            std::chrono::system_clock::time_point begin_time;
            std::chrono::system_clock::time_point end_time;
            std::list<std::shared_ptr<error_type>> errors;

        public:
            error_info &operator=(const std::pair<int, std::string> &err_code_msg)
            {
                errors.emplace_back(std::make_shared<error_type>(error_type{err_code_msg.first, err_code_msg.second}));
                return *this;
            }
            operator std::pair<int, std::string>()
            {
                return std::make_pair(errors.back()->code, errors.back()->msg);
            }
    };
    class error_resigter
    {
        public:
            std::list<std::shared_ptr<error_info>> error_list;
            std::shared_ptr<error_info> last_error;
    };

    // 日志基类
    class logger
    {
    public:
        enum class level
        {
            debug,
            info,
            warn,
            error,
            fatal
        };
        logger() = default;
        virtual void write(level lv, const char *msg) { return; }
        virtual void log(error_type err) { return; }
        // 用于启用一次计时测量
        virtual void perl(std::string perl_label) { return; }
        virtual void perl_end(std::string perl_label = "") { return; }
    };


} // namespace tianli::global
