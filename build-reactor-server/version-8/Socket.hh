/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#pragma once

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "util.hh"
#include "InetAddress.hh"

namespace yichen
{

    class InetAddress;

    class Socket {
    private:
        int fd;

    public:
        Socket();
        Socket(int);
        ~Socket();

        inline void m_bind(InetAddress* _addr) {
            struct sockaddr_in addr = _addr->getAddr();
            socklen_t addr_len = _addr->getAddr_len();
            DEBUG(bind(fd, (sockaddr*)&addr, addr_len) == -1, "socket bind error");
            _addr->setInetAddr(addr, addr_len);
        }
        
        inline void m_listen(){
            DEBUG(listen(fd, SOMAXCONN) == -1, "socket listen error");
        }

        inline void setnonblocking() {
            fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
        }

        inline int m_accept(InetAddress* _addr) {
            struct sockaddr_in addr;
            socklen_t addr_len = sizeof(addr);
            bzero(&addr, sizeof(addr));
            int clnt_sockfd = accept(fd, (sockaddr*)&addr, &addr_len);
            DEBUG(clnt_sockfd == -1, "socket accept error");
            _addr->setInetAddr(addr, addr_len);
            return clnt_sockfd;
        }

        inline int m_getFd() {
            return fd;
        }
    };

}