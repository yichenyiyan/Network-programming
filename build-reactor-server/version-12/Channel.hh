/**
 * @file Channel.hh
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
    class Socket;
    class EventLoop;

    class Channel {
    public:
        Channel(EventLoop *loop, int fd);
        ~Channel();

        DISALLOW_COPY_AND_MOVE(Channel)

        void HandleEvent();
        void EnableRead();

        int GetFd();
        uint32_t GetListenEvents();
        uint32_t GetReadyEvents();
        bool GetInEpoll();
        void SetInEpoll(bool in = true);
        void UseET();

        void SetReadyEvents(uint32_t ev);
        void SetReadCallback(std::function<void()> const &callback);

    private:
        EventLoop *loop_;
        int fd_;
        uint32_t listen_events_;
        uint32_t ready_events_;
        bool in_epoll_;
        std::function<void()> read_callback_;
        std::function<void()> write_callback_;
        
    };
}