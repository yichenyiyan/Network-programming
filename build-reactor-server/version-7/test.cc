#include "Server.hh"
#include "EventLoop.hh"

using namespace yichen;

static constexpr int main_ret = 0;

int 
main(int argc, char** argv) {
    EventLoop* loop = new EventLoop(); 
    Server* server = new Server(loop);
    loop->loop();

    return main_ret;
}