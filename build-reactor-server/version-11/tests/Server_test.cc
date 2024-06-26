/**
 * @file Server_test.cc
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */


#include "../include/Server.hh"
#include "../include/EventLoop.hh"

static const int main_ret = 0;

using namespace yichen;

int 
main(int argc, char** argv) {
  EventLoop *loop = new EventLoop();
  Server *server = new Server(loop);
  loop->loop();
  
  delete server;
  delete loop;

  return main_ret;
}
