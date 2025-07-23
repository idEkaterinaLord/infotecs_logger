#pragma once

#include <string>

namespace logger {

enum class LogLevel {
    ERROR,
    WARNING,
    INFO
};

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void log(const std::string& message, LogLevel level) = 0;
    virtual void setLevel(LogLevel level) = 0;
};

} // namespace logger
