#include "logger/FileLogger.hpp"
#include "logger/ILogger.hpp"
#include "SafeQueue.hpp"
#include "logger/SocketLogger.hpp"

#include <iostream>
#include <thread>
#include <atomic>
#include <sstream>

using namespace logger;

struct LogEntry {
    std::string message;
    LogLevel level;
};

SafeQueue<LogEntry> logQueue;
std::atomic<bool> running{true};

void loggerThreadFunc(ILogger& logger) {
    while (running) {
        auto entryOpt = logQueue.pop();
        if (entryOpt) {
            const auto& entry = *entryOpt;
            logger.log(entry.message, entry.level);
        }
    }
}

LogLevel parseLevel(const std::string& input) {
    if (input == "error") return LogLevel::ERROR;
    if (input == "warning") return LogLevel::WARNING;
    return LogLevel::INFO;
}

int main(int argc, char* argv[]) {
    std::string log_filename = "log.txt";
    LogLevel default_level = LogLevel::INFO;

    if (argc >= 2) log_filename = argv[1];
    if (argc >= 3) default_level = parseLevel(argv[2]);

    //std::unique_ptr<ILogger> logger = std::make_unique<FileLogger>(log_filename, default_level);
    //std::unique_ptr<ILogger> logger = std::make_unique<SocketLogger>("127.0.0.1", 5000, default_level);
    std::unique_ptr<ILogger> logger;

    std::string mode = "file"; // по умолчанию
    if (argc >= 4) mode = argv[3];

    if (mode == "socket") {
        logger = std::make_unique<SocketLogger>("127.0.0.1", 5000, default_level);
    } else {
        logger = std::make_unique<FileLogger>(log_filename, default_level);
    }

    std::thread logger_thread(loggerThreadFunc, std::ref(*logger));
    
    std::cout << "Режим логгирования: " << (mode == "socket" ? "сокет" : "файл") << std::endl;

    std::cout << "Введите сообщение (или 'exit' для выхода):" << std::endl;

    std::string line;
    while (std::getline(std::cin, line)) {
        //if (line == "exit") break;
        std::istringstream iss(line);
        std::string level_str, rest;
        iss >> level_str;
        if (level_str == "exit") break;
        std::getline(iss, rest);
        std::string message = rest.empty() ? level_str : rest.substr(1);

        std::istringstream iss(line);
        std::string level_str, rest;
        iss >> level_str;
        std::getline(iss, rest);
        std::string message = rest.empty() ? level_str : rest.substr(1);;

        LogLevel level = rest.empty() ? default_level : parseLevel(level_str);
        logQueue.push({message, level});
    }

    running = false;
    logQueue.stop();
    logger_thread.join();
    return 0;
}
