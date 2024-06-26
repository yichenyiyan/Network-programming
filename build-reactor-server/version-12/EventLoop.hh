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

#include "util.hh"

#include <functional>

namespace yichen
{
    class Epoll;
    class Channel;
    
    class EventLoop {
    public:
        EventLoop();
        ~EventLoop();

        DISALLOW_COPY_AND_MOVE(EventLoop)

        void Loop();
        void UpdateChannel(Channel *ch);

    private:
        Epoll* epoll_;
        bool quit_;
        
    };
}