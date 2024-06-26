/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#pragma once

#include <unordered_map>

namespace yichen
{
    class EventLoop;
    class Socket;
    class Acceptor;
    class Connection;

    class Server {
    private:
        EventLoop *loop;
        Acceptor *acceptor;
        std::unordered_map<int, Connection*> connections;

    public:
        Server(EventLoop*);
        ~Server();

        void handleReadEvent(int);
        void newConnection(Socket* sock);
        void deleteConnection(int sock);
    };
}

