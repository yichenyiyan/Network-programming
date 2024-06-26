/**
 * @file Epoll.hh
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */


#pragma once

#include <vector>
#include <sys/epoll.h>

#include "util.hh"
#include <string.h>


namespace yichen
{

    class Channel;

    class Epoll {
    private:
        int epfd;
        struct epoll_event *events;
        Epoll(const Epoll&) = delete;
        Epoll& operator=(const Epoll&) = delete;

    public:
        Epoll();
        ~Epoll();

        #if 0
        inline void addFd(int fd, uint32_t op) {
            struct epoll_event ev;
            bzero(&ev, sizeof(ev));
            ev.data.fd = fd;
            ev.events = op;
            DEBUG(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event error");
        }
        #endif
        
        void updateChannel(Channel* ch);
        void deleteChannel(Channel* ch);
        
        std::vector<Channel*> poll(int timeout = -1);
    };

}