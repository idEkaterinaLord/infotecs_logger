#include "logger/FileLogger.hpp"

namespace logger {

FileLogger::FileLogger(const std::string& filename, LogLevel level)
    : current_level_(level)
{
    log_file_.open(filename, std::ios::app);
}

void FileLogger::setLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    current_level_ = level;
}

void FileLogger::log(const std::string& message, LogLevel level) {
    if (level > current_level_) return;

    std::lock_guard<std::mutex> lock(mutex_);
    if (!log_file_.is_open()) return;

    log_file_ << getCurrentTime() << " | "
              << levelToString(level) << " | "
              << message << std::endl;
}

std::string FileLogger::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    char buf[26];
    ctime_r(&t, buf);
    buf[24] = '\0'; // remove newline
    return std::string(buf);
}

std::string FileLogger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::INFO: return "INFO";
        default: return "UNKNOWN";
    }
}

} // namespace logger