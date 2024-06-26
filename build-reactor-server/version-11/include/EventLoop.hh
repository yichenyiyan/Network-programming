/**
 * @file EventLoop.hh
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */



#pragma once
#include <functional>


namespace yichen
{
    class Epoll;
    class Channel;
    
    class EventLoop {
    private:
        Epoll* ep;
        bool quit;
        EventLoop(const EventLoop&) = delete;
        EventLoop& operator=(const EventLoop&) = delete;

    public:
        EventLoop();
        ~EventLoop();

        void loop();
        void updateChannel(Channel*);

        void addThread(std::function<void()>);
    };
}
