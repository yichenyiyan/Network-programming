/**
 * @file Client_test.cc
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */


#include <unistd.h>
#include <cstring>
#include <iostream>

#include "../include/Buffer.hh"
#include "../include/Socket.hh"
#include "../include/util.hh"

using namespace yichen;

static constexpr int main_ret = 0;
static constexpr int buffer_size = 1024;

int 
main(int argc, char** argv) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    DEBUG(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(1234);

    DEBUG(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket connect error");
    
    while (true) {
        char buf[buffer_size];  
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if (write_bytes == -1) {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            printf("message from server: %s\n", buf);
        } 
        else if (read_bytes == 0) {
            printf("server socket disconnected!\n");
            break;
        }
        else if (read_bytes == -1) {
            close(sockfd);
            DEBUG(true, "socket read error");
        }
    }
    close(sockfd);

    return main_ret;
}
