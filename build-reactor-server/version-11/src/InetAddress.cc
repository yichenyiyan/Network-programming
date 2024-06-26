/**
 * @file InetAddress.cc
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */


#include <string.h>

#include "../include/InetAddress.hh"

yichen::InetAddress::InetAddress() {
    bzero(&addr, sizeof(addr));
}

yichen::InetAddress::InetAddress(const char* ip, uint16_t port) {
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
}

yichen::InetAddress::~InetAddress() {}
