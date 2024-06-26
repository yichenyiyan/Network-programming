/**
 * @file Buffer.cc
 * @author yichenyan (2513626988@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright （严亦辰）2024
 *
 */

#include "Buffer.hh"

#include <cstring>

void 
yichen::Buffer::Append(const char *str, int size) {
  for (int i = 0; i < size; ++i) {
    if (str[i] == '\0') {
      break;
    }
    buf_.push_back(str[i]);
  }
}





