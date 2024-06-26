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
    
    class Connection {
    private:
        EventLoop *loop;
        Socket *sock;
        Channel *channel;
        std::function<void(Socket*)> deleteConnectionCallback;
        
    public:
        Connection(EventLoop *_loop, Socket *_sock);
        ~Connection();
        
        void echo(int sockfd);
        
        inline void setDeleteConnectionCallback(std::function<void(Socket*)> _cb) {
            deleteConnectionCallback = _cb;
        }
    };

}