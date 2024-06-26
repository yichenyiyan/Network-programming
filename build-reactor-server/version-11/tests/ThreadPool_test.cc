/**
 * @file ThreadPool_test.cc
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */

#include <iostream>
#include <string>

#include "../include/ThreadPool.hh"

using namespace yichen;

static const int main_ret = 0;

void Print(int a, double b, const char *c, std::string const &d) { std::cout << a << b << c << d << std::endl; }

void Test() { std::cout << "help" << std::endl; }

int 
main(int argc, char const *argv[]) {
  ThreadPool *poll = new ThreadPool();
  std::function<void()> func = std::bind(Print, 1, 3.14, "hello", std::string("world"));
  poll->add(func);
  func = Test;
  poll->add(func);
  delete poll;

  return main_ret;
}
