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

#include "util.hh"

#include <functional>

namespace yichen
{
    class EventLoop;
    class Socket;
    class Channel;

    class Acceptor final {
    public:
        explicit Acceptor(EventLoop* loop);
        ~Acceptor();

        DISALLOW_COPY_AND_MOVE(Acceptor)

        void AcceptConnection();
        void SetNewConnectionCallback(std::function<void(Socket*)> const &callback);

    private:
        EventLoop* loop_;
        Socket* sock_;
        Channel* channel_;
        std::function<void(Socket*)> new_connection_callback_;
    };
}
