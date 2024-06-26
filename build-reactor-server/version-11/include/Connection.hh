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

#include <string>
#include <functional>

namespace yichen
{
    class EventLoop;
    class Socket;
    class Channel;
    class Buffer;
    
    class Connection {
    private:
        std::string *inBuffer;
        Buffer *readBuffer;  
        EventLoop *loop;
        Socket *sock;
        Channel *channel;
        std::function<void(int)> deleteConnectionCallback;
        Connection(const Connection&) = delete;
        Connection& operator=(const Connection&) = delete;
        
    public:
        Connection(EventLoop *_loop, Socket *_sock);
        ~Connection();
        
        void echo(int sockfd);
        
        inline void setDeleteConnectionCallback(std::function<void(int)> _cb) {
            deleteConnectionCallback = _cb;
        }

        void send(int sockfd);
    };

}