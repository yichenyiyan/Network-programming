/**
 * @file Socket.cc
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */


#include "../include/Socket.hh"


yichen::Socket::Socket() : fd(-1) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    DEBUG(fd == -1, "socket create error");
}

yichen::Socket::Socket(int _fd) : fd(_fd) {
    DEBUG(fd == -1, "socket create error");
}

yichen::Socket::~Socket() {
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}
