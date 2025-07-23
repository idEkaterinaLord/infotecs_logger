#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template<typename T>
class SafeQueue {
public:
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
        cond_var_.notify_one();
    }

    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [&] { return !queue_.empty() || stop_; });

        if (queue_.empty()) return std::nullopt;

        T val = queue_.front();
        queue_.pop();
        return val;
    }

    void stop() {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;
        cond_var_.notify_all();
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
    bool stop_ = false;
};
