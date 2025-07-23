#include "logger/SocketLogger.hpp"
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

namespace logger {

SocketLogger::SocketLogger(const std::string& host, int port, LogLevel level)
    : host_(host), port_(port), current_level_(level)
{
    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ < 0) {
        std::cerr << "Ошибка создания сокета\n";
    }
}

SocketLogger::~SocketLogger() {
    if (sockfd_ >= 0) {
        close(sockfd_);
    }
}

void SocketLogger::setLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    current_level_ = level;
}

void SocketLogger::log(const std::string& message, LogLevel level) {
    if (level > current_level_) return;

    std::lock_guard<std::mutex> lock(mutex_);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    inet_pton(AF_INET, host_.c_str(), &addr.sin_addr);

    std::string payload = message + "\n";
    sendto(sockfd_, payload.c_str(), payload.size(), 0,
           (struct sockaddr*)&addr, sizeof(addr));
}

} // namespace logger
