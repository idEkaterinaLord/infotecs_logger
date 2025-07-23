#pragma once

#include "ILogger.hpp"
#include <fstream>
#include <mutex>
#include <string>

namespace logger {

class FileLogger : public ILogger {
public:
    FileLogger(const std::string& filename, LogLevel level);
    void log(const std::string& message, LogLevel level) override;
    void setLevel(LogLevel level) override;

private:
    std::ofstream log_file_;
    LogLevel current_level_;
    std::mutex mutex_;

    std::string getCurrentTime() const;
    std::string levelToString(LogLevel level) const;
};

} // namespace logger
