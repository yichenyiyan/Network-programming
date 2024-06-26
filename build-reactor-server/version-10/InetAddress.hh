/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#pragma once

#include <arpa/inet.h>

namespace yichen
{
    class InetAddress {
    private:
        struct sockaddr_in addr;
        //socklen_t addr_len;

    public:
        InetAddress();
        InetAddress(const char* ip, uint16_t port);
        ~InetAddress();

    public:
        inline void setInetAddr(sockaddr_in _addr) {
            addr = _addr;
            //addr_len = _addr_len;
        }

        inline sockaddr_in getAddr() {
            return addr;
        }
    };

}