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

#include <vector>
#include <unordered_map>

namespace yichen
{
    class EventLoop;
    class Socket;
    class Acceptor;
    class Connection;
    class ThreadPool;

    class Server {
    private:
        EventLoop* main_Reactor;
        Acceptor* acceptor;
        ThreadPool* threadPool;
        std::vector<EventLoop*> sub_Reactors;
        std::unordered_map<int, Connection*> connections;

        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;

    public:
        Server(EventLoop*);
        ~Server();

        void handleReadEvent(int);
        void newConnection(Socket* sock);
        void deleteConnection(int sock);
    };
}

