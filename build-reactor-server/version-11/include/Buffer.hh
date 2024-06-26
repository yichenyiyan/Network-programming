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

#include <string>

namespace yichen
{
    class Buffer {
    private:
        std::string buf;
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
        
    public:
        Buffer();
        ~Buffer();
    
    public:
        inline void append(const char* _str, int _size) {
            for (int i = 0; i < _size; ++i) {
                if(_str[i] == '\0') break;
                buf.push_back(_str[i]);
            }
        }

        inline ssize_t size() {
            return buf.size();
        }

        inline const char* c_str() {
            return buf.c_str();
        }
        
        inline void clear() {
            buf.clear();
        }
    };
}


