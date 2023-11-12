#ifndef VLOG_MESSAGE_H_
#define VLOG_MESSAGE_H_

#include <ostream>
#include <mutex>
#include <sstream>

namespace vlog
{

enum class log_level {
    LV_ERROR,
    LV_WARNING,
    LV_INFO,
    LV_DEBUG,
    LV_VERBOSE
};
    
class message : public std::ostream {
public:
    message(log_level _level);
    ~message();

private:
    class buffer : public std::streambuf {
    public:
        int_type overflow(int_type);
        std::streamsize xsputn(const char*, std::streamsize);
        std::stringstream data_;
    };

    std::chrono::system_clock::time_point when_;
    buffer buffer_;
    log_level level_;
    static std::mutex mutex_;
};
}

#define VLOG_ERROR      vlog::message(vlog::log_level::LV_ERROR)
#define VLOG_WARNING    vlog::message(vlog::log_level::LV_WARNING)
#define VLOG_INFO       vlog::message(vlog::log_level::LV_INFO)
#define VLOG_DEBUG      vlog::message(vlog::log_level::LV_DEBUG)
#define VLOG_VERBOSE    vlog::message(vlog::log_level::LV_VERBOSE)

#endif // !VLOG_MESSAGE_H_
