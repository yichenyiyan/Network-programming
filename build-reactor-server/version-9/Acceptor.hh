/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

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

    public:
        Acceptor(EventLoop *_loop);
        ~Acceptor();

        void acceptConnection();
        
        inline void setNewConnectionCallback(std::function<void(Socket*)> _cb) {
            newConnectionCallback = _cb;
        }
    };

}