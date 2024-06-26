#include "Socket.hh"


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
