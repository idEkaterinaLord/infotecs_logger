#pragma once

#include "ILogger.hpp"
#include <mutex>
#include <string>

namespace logger {

class SocketLogger : public ILogger {
public:
    SocketLogger(const std::string& host, int port, LogLevel level);
    ~SocketLogger();

    void log(const std::string& message, LogLevel level) override;
    void setLevel(LogLevel level) override;

private:
    int sockfd_;
    std::string host_;
    int port_;
    LogLevel current_level_;
    std::mutex mutex_;
};

} // namespace logger
