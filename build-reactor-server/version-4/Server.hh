/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#pragma once

namespace yichen
{
    class EventLoop;
    class Socket;

    class Server {
    private:
        EventLoop *loop;

    public:
        Server(EventLoop*);
        ~Server();

        void handleReadEvent(int);
        void newConnection(Socket* serv_sock);
    };
}

