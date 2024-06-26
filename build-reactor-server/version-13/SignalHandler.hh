/**
 * @file SignalHandler.hh
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */

#pragma once

#include <map>
#include <signal.h>
#include <functional>

#include "util.hh"

namespace yichen
{
    std::map<int, std::function<void()>> handlers_;

    void signal_handler(int sig) {
        handlers_[sig]();
    }

    struct Signal final {
        Signal() = default;
        ~Signal() = default;

        DISALLOW_COPY_AND_MOVE(Signal)

        static void signal(int sig, const std::function<void()> &handler) {
            handlers_[sig] = handler;
            ::signal(sig, signal_handler);
        }
    };
}