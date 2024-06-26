/**
 * @file Acceptor.hh
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
    class EventLoop;
    class Socket;
    class Channel;

    class Acceptor {
    private:
        EventLoop* loop;
        Socket* sock;
        Channel* acceptChannel;
        std::function<void(Socket*)> newConnectionCallback;

        Acceptor(const Acceptor&) = delete;
        Acceptor& operator=(const Acceptor&) = delete;

    public:
        Acceptor(EventLoop *_loop);
        ~Acceptor();

        void acceptConnection();
        
        inline void setNewConnectionCallback(std::function<void(Socket*)> _cb) {
            newConnectionCallback = _cb;
        }
    };

}