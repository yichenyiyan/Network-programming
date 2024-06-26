/**
 * @file util.hh
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */

#pragma once

#include <cstdlib>   /*exit*/
#include <cassert>   /*assert*/
#include <stdexcept>

#define OS_LINUX   /*only develop for Linux OS*/

namespace yichen
{
    #define DEBUG(expr, errmsg)  if ((expr)) { perror(errmsg); exit(EXIT_FAILURE); }

    // Macros to disable copying and moving
    #define DISALLOW_COPY(cname)       cname(const cname&) = delete; cname& operator=(const cname&) = delete; 

    #define DISALLOW_MOVE(cname)       cname(cname&&) = delete; cname& operator=(cname&&) = delete; 

    #define DISALLOW_COPY_AND_MOVE(cname)   DISALLOW_COPY(cname) DISALLOW_MOVE(cname) 

    #define ASSERT(expr, message) assert((expr) && (message))

    #define UNREACHABLE(message) throw std::logic_error(message)
}

