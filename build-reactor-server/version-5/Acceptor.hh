/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#pragma once

#include <functional>

namespace yichen
{
    class EventLoop;
    class Socket;
    class InetAddress;
    class Channel;

    class Acceptor {
    private:
        EventLoop* loop;
        Socket* sock;
        InetAddress* addr;
        Channel* acceptChannel;
        std::function<void(Socket*)> newConnectionCallback;

    public:
        Acceptor(EventLoop *_loop);
        ~Acceptor();

        inline void acceptConnection() {
            newConnectionCallback(sock);
        }
        

        inline void setNewConnectionCallback(std::function<void(Socket*)> _cb) {
            newConnectionCallback = _cb;
        }
    };

}