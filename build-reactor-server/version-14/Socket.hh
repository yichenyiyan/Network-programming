/**
 * @file Socket.hh
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */

#pragma once

#include "util.hh"

#include <arpa/inet.h>

namespace yichen
{
    class InetAddress final {
    public:
        InetAddress();
        InetAddress(const char* ip, uint16_t port);
        ~InetAddress() = default;

        DISALLOW_COPY_AND_MOVE(InetAddress)

        void SetAddr(sockaddr_in addr);
        sockaddr_in GetAddr();
        const char *GetIp();
        uint16_t GetPort();

    private:
        struct sockaddr_in addr_ {};
    };

    class Socket final {
    private:
        int fd_;

    public:
        Socket();
        explicit Socket(int fd);
        ~Socket();

        DISALLOW_COPY_AND_MOVE(Socket)

        void Bind(InetAddress* addr);
        void Listen();
        int Accept(InetAddress* addr);

        void Connect(InetAddress* addr);
        void Connect(const char* ip, uint16_t port);

        void SetNonBlocking();
        bool IsNonBlocking();
        int GetFd();
    };
}