/**
 * @file Connection.hh
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
    class Buffer;
    class Socket;
    class Channel;
    class EventLoop;

    class Connection final {
    public:
        // status code
        enum class State : const int {
            Invalid = 1,
            Handshaking,
            Connected,
            Closed,
            Failed
        };

        Connection(EventLoop* loop, Socket* sock);
        ~Connection();

        DISALLOW_COPY_AND_MOVE(Connection)

        void Read();
        void Write();

        void SetDeleteConnectionCallback(std::function<void(Socket*)> const &callback);
        void SetOnConnectCallback(std::function<void(Connection*)> const &callback);
        State GetState();
        void Close();
        void SetSendBuffer(const char *str);
        Buffer* GetReadBuffer();
        const char *ReadBuffer();
        Buffer* GetSendBuffer();
        const char *SendBuffer();
        void GetlineSendBuffer();
        Socket* GetSocket();

        void OnConnect(std::function<void()> fn);

    private:
        EventLoop* loop_;
        Socket* sock_;
        Channel* channel_;
        State state_;
        Buffer* read_buffer_;
        Buffer* send_buffer_;
        std::function<void(Socket*)> delete_connectioin_callback_;
        std::function<void(Connection*)> on_connect_callback_;

        void ReadNonBlocking();
        void WriteNonBlocking();
        void ReadBlocking();
        void WriteBlocking();

    };
}