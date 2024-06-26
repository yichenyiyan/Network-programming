/**
 * @file test_server.cc
 * @author yichenyan (2513626988@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright （严亦辰）2024
 *
 */

#include <iostream>
#include "Server.hh"
#include "Buffer.hh"
#include "Socket.hh"
#include "EventLoop.hh"
#include "Connection.hh"


using namespace yichen;

static constexpr int main_ret = 0;


int 
main(int argc, char** argv) {
    EventLoop* loop = new EventLoop();
    Server* server = new Server(loop);
    server->OnConnect([](Connection* conn) {
        conn->Read();
        if (conn->GetState() == Connection::State::Closed) {
        conn->Close();
        return;
        }
        std::cout << "Message from client " << conn->GetSocket()->GetFd() << ": " << conn->ReadBuffer() << std::endl;
        conn->SetSendBuffer(conn->ReadBuffer());
        conn->Write();
    });

    loop->Loop();
    delete server;
    delete loop;
    return main_ret;
}
