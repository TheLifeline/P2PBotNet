#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "peer.h"
int init_client(const char* target_addr,const int target_port)
{
    int clientSock;
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(target_port);
    client.sin_addr.s_addr = inet_addr(target_addr);
    if ((clientSock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("clientSock socket error\n");
        return -1;
    }
    //向感染源发起连接
    while(connect(clientSock,(struct sockaddr*)&client,sizeof(struct sockaddr)) == -1);
    return clientSock;
}

// get the list of state, fileNames, fileSize that Peer Wants to Publish
// registeer the peer with the Server/Tracker along with the list of files that the peer wishes to publish
int registerPeer(int clientSock, int listeningPort) {
    char buffer[BUFFER_SIZE_NETWORK];
    int retVal = 0;

    char *command="RegisterPeer";

    Message m;
    m.state = 200;
    m.action = 1;
    m.command = malloc(strlen(command) + sizeof(int) + 1);
    sprintf(m.command, "%s,%d",command,listeningPort);

    char *encodedStr = encodeMessage(m);
    sendMessage(clientSock, encodedStr);
    free(encodedStr);
    char *receivedMessage = receiveMessage(clientSock, buffer);
    Message response = decodeMessage(receivedMessage);
    printf("[CLIENT]server response is:%s\n", response.command);
    free(receivedMessage);
    return retVal;
}