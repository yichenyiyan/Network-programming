/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#pragma once 

#include <string>

namespace yichen
{
    class Buffer {
    private:
        std::string buf;
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


