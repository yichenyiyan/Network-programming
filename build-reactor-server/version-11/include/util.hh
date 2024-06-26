/**
 * @file FileName.hh
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */



#pragma once 

#include <cstdio>
#include <cstdlib>

namespace yichen
{
#define DEBUG(expr, errmsg)  if ((expr)) { perror(errmsg); exit(EXIT_FAILURE); }
}


