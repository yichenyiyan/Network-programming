/**
 * @file Server.hh
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */

#pragma once

#include <map>
#include <vector>
#include <functional>

#include "util.hh"


namespace yichen 
{
    class EventLoop;
    class Socket;
    class Acceptor;
    class Connection;
    class ThreadPool;

    class Server final {
    private:
        EventLoop* main_reactor_;
        Acceptor* acceptor_;
        std::map<int, Connection*> connections_;
        std::vector<EventLoop*> sub_reactors_;
        ThreadPool* thread_pool_;
        std::function<void(Connection*)> on_connect_callback_;

    public:
        explicit Server(EventLoop* loop);
        ~Server();

        DISALLOW_COPY_AND_MOVE(Server)

        void NewConnection(Socket* sock);
        void DeleteConnection(Socket* sock);
        void OnConnect(std::function<void(Connection*)> fn);
    };

}