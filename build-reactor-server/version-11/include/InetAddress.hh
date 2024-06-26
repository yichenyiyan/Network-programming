/**
 * @file InetAddress.hh
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */


#pragma once

#include <arpa/inet.h>

namespace yichen
{
    class InetAddress {
    private:
        struct sockaddr_in addr;
        InetAddress(const InetAddress&) = delete;
        InetAddress& operator=(const InetAddress&) = delete;

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