/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

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

    public:
        Server(EventLoop*);
        ~Server();

        void handleReadEvent(int);
        void newConnection(Socket* sock);
        void deleteConnection(int sock);
    };
}

