/**
 * @file Buffer.hh
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

#include <string>
#include <iostream>

namespace yichen
{
    class Buffer final {
    public:
        Buffer() = default;
        ~Buffer() = default;

        DISALLOW_COPY_AND_MOVE(Buffer)

        void Append(const char *_str, int _size);

        inline ssize_t Size() {
            return buf_.size();
        }

        inline const char* ToStr() {
            return buf_.c_str();
        }
        
        inline void Clear() {
            buf_.clear();
        }

        inline void Getline() {
            buf_.clear();
            std::getline(std::cin, buf_);
        }

        inline void SetBuf(const char* buf) {
            buf_.clear();
            buf_.append(buf);
        }

    private:
        std::string buf_;
    };

}