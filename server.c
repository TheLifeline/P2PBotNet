#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "peer.h"
#include "server.h"
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int init_server(const int ListeningPort)
{
    int server_sock;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(ListeningPort);
    server.sin_addr.s_addr = INADDR_ANY;
    memset(server.sin_zero, '\0', sizeof(server.sin_zero));
    // inititalize peerserver socket descriptor
    if ((server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("server socket error\n");
        return -1;
    }
    if (bind(server_sock, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1) {
        printf("binding error\n");
        return -1;
    }
        //起监听
    if (listen(server_sock, MAX_CLIENTS) == -1) {
        printf("listening error");
        exit(-1);
    }
    printf("[SERVER]Start Listen!!\n");
    return server_sock;
}

// 作为服务端的处理线程
void* threadedServer(void *arg){
    int server_socket = *((int*)arg);
    int client_socket;
    struct sockaddr_in client;
    socklen_t clnt_addr_size = sizeof(struct sockaddr_in);
    if ((client_socket = accept(server_socket, (struct sockaddr *) &client, &clnt_addr_size)) == -1) {
            printf("accepting error");
            exit(-1);
    }
    printf("[SERVER]Get the Client;IP:%s\n",inet_ntoa(client.sin_addr));
    char buffer[BUFFER_SIZE_NETWORK];
    while (1) {
        char *receivedMessage = receiveMessage(client_socket, buffer);
        printf("[SERVER]Get the Client Message.\n");
        if (receivedMessage != NULL && strlen(receivedMessage)!=0) {
            // serveChunk:
            printf("[SERVER]Start process Message:%s,len:%ld\n",receivedMessage,strlen(receivedMessage));
            Message receivedMsg = decodeMessage(receivedMessage);
            printf("[SERVER]server get args is:%s\n",receivedMsg.command);
            free(receivedMessage);
            //todo:处理发过来的字段
            switch (receivedMsg.action) {
                case 1:
                    receivedMsg.command="SUCCESS";
                    sendMessage(client_socket,encodeMessage(receivedMsg));
                    break;
            }
        }else{
            break;
        }
    }
}
