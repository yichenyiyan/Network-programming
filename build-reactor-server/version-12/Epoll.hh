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

#include "util.hh"

#include <vector>

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif


namespace yichen
{
    class Channel;

    class Epoll final {
    public:
        Epoll();
        ~Epoll();

        DISALLOW_COPY_AND_MOVE(Epoll)

        void UpdateChannel(Channel *ch);
        void DeleteChannel(Channel *ch);

        std::vector<Channel *> Poll(int timeout = -1);

    private:
        int epfd_{1};
        struct epoll_event *events_{nullptr};
    };
}