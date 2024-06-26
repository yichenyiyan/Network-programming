/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#include <string.h>

#include "InetAddress.hh"

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
