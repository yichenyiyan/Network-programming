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


#include <mutex>
#include <queue>
#include <thread>
#include <future>
#include <vector>
#include <functional>
#include <condition_variable>

namespace yichen 
{
    class ThreadPool {
    private:
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> tasks;
        std::mutex tasks_mtx;
        std::condition_variable cv;
        bool stop;
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
        
    public:
        ThreadPool(int size = 10);
        ~ThreadPool();

        //void add(std::function<void()>);
        template <class T, class... Args>
        auto add(T&& t, Args&&... args) 
        -> std::future<typename std::result_of<T(Args...)>::type>;
    };
   
}

template <class T, class... Args>
auto yichen::ThreadPool::add(T&& t, Args&&... args) 
-> std::future<typename std::result_of<T(Args...)>::type> {
    using return_type = typename std::result_of<T(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
        std::bind(std::forward<T>(t), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();

    {
        std::unique_lock<std::mutex> lock(tasks_mtx);

        if (stop)
            throw std::runtime_error("the ThreadPool has stop!");
        
        tasks.emplace([task]() { (*task)(); });
    }

    cv.notify_one();

    return res;
}

