/**
 * @file ThreadPool.hh
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */

#pragma once

#include <condition_variable>  // NOLINT
#include <functional>
#include <future>  // NOLINT
#include <memory>
#include <mutex>  // NOLINT
#include <queue>
#include <thread>  // NOLINT
#include <utility>
#include <vector>
#include "util.hh"

namespace yichen
{
    class ThreadPool final {
    public:
        explicit ThreadPool(unsigned size = std::thread::hardware_concurrency());
        ~ThreadPool();

        DISALLOW_COPY_AND_MOVE(ThreadPool)

        template <class F, class... Args>
        auto Add(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
        
    private:
        std::vector<std::thread> workers_;
        std::queue<std::function<void()>> tasks_;
        std::mutex queue_mutex_;
        std::condition_variable condition_variable_;
        bool stop_{ false };
    };

}

template <class F, class... Args>
auto yichen::ThreadPool::Add(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task =
      std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);

    // don't allow enqueueing after stopping the pool
    if (stop_) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }

    tasks_.emplace([task]() { (*task)(); });
  }
  condition_variable_.notify_one();
  return res;
}