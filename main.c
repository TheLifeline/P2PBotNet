#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "peer.h"
#include "client.h"
#include "server.h"

int main(int argc, char **argv) {
    //argv1:感染元IP
    //argv2:感染元端口
    // Accept the Inputs and call the init Function
    if(argc<3){
        printf("You must input two args.<source IP> <source PORT>\n");
        return 0;
    }
    int serverListeningPort = 9999;//会在节点注册的时候传给服务端
    char* metaIp = argv[1];
    int metaPort = atoi(argv[2]);
    if (strcmp(metaIp,"0.0.0.0")!=0)
    {     
        //作为客户端
        int clientSock=init_client(metaIp, metaPort);
        //向感染源请求注册
        registerPeer(clientSock, serverListeningPort);
        //todo:将感染源添加进路由器
        //循环更新路由表
        // while (1) {
        // }
    }
    printf("only server!\n");

    //作为服务器
    int server_socket = init_server(serverListeningPort);
    if (server_socket != -1)
    {
        while (1)
        {     
            pthread_t tid; // only one thread to have a non-blocking listening to multiple clients
            //起一个线程去处理发送到该服务器的请求与命令
            if (pthread_create(&tid, NULL, threadedServer, (void *)(&server_socket)) != 0) {
                printf("Failed to create thread\n");
            }
        }
    }
    return 0;
}