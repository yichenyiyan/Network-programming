/**
 * @file ThreadPool.cc
 * @author yichenyan (2513626988@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright （严亦辰）2024
 *
 */

#include "ThreadPool.hh"

yichen::ThreadPool::ThreadPool(unsigned size) {
  for (unsigned i = 0; i < size; ++i) {
    workers_.emplace_back(std::thread([this]() {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(queue_mutex_);
          condition_variable_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });
          if (stop_ && tasks_.empty()) {
            return;
          }
          task = tasks_.front();
          tasks_.pop();
        }
        task();
      }
    }));
  }
}

yichen::ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }
  condition_variable_.notify_all();
  for (std::thread &th : workers_) {
    if (th.joinable()) {
      th.join();
    }
  }
}
