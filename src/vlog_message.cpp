#include "vlog_message.h"
#include <iostream>
#include <iomanip>

namespace vlog
{

std::mutex message::mutex_;

message::message(log_level _level)
    : buffer_()
    , std::ostream(&buffer_)
    , level_(_level) {
        when_ = std::chrono::system_clock::now();
    
}

message::~message() try {
    const char *its_level;
    switch (level_) {
        case log_level::LV_ERROR:
            its_level = "error";
            break;
        case log_level::LV_WARNING:
            its_level = "warning";
            break;
        case log_level::LV_INFO:
            its_level = "info";
            break;
        case log_level::LV_DEBUG:
            its_level = "debug";
            break;
        case log_level::LV_VERBOSE:
            its_level = "verbose";
            break;
        default:
            its_level = "none";
    };

    auto its_time_t = std::chrono::system_clock::to_time_t(when_);
    struct tm its_time;
    localtime_r(&its_time_t, &its_time);
    auto its_ms = (when_.time_since_epoch().count() / 100) % 1000000;

    std::cout
        << std::dec
        << std::setw(4) << its_time.tm_year + 1900 << "-"
        << std::setfill('0')
        << std::setw(2) << its_time.tm_mon + 1 << "_"
        << std::setw(2) << its_time.tm_mday << " "
        << std::setw(2) << its_time.tm_hour << ":"
        << std::setw(2) << its_time.tm_min << ":"
        << std::setw(2) << its_time.tm_sec << "."
        << std::setw(6) << its_ms << " ["
        << its_level << "] "
        << buffer_.data_.str()
        << std::endl;

} catch (const std::exception& e) {
    std::cout <<"\nvlog: Error destroying message class: " << e.what() << '\n';
}

std::streambuf:: int_type 
message::buffer::overflow(std:: streambuf::int_type c) {
    if(c != EOF) {
        data_ << (char)c;
    }

    return c;
}

std::streamsize
message::buffer::xsputn(const char *s, std::streamsize n) {
    data_.write(s, n);
    return n;
}
    
} // namespace vlog