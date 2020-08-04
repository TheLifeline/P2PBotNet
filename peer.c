//
// Created by user on 2020/8/4.
//
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "peer.h"
#include "global.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// get the list of state, fileNames, fileSize that Peer Wants to Publish
// registeer the peer with the Server/Tracker along with the list of files that the peer wishes to publish
int registerPeer(int serverSock, int listeningPort) {
    char buffer[BUFFER_SIZE_NETWORK];
    int retVal = 0;

    char *args="RegisterPeer";

    Message m;
    m.state = 200;
    m.action = 1;
    m.arguments = malloc(strlen(args) + sizeof(int) + 1);
    sprintf(m.arguments, "%d,%s",listeningPort, args);

    char *encodedStr = encodeMessageServer(m);
    sendMessage(serverSock, encodedStr);
    free(encodedStr);
    char *receivedMessage = receiveMessage(serverSock, buffer);
    printf("[CLIENT]server response is:%s\n", receivedMessage);
    free(receivedMessage);
    return retVal;
}

// decode the message sent by Server and call the respective functions to get the desired functionality
// in simple terms, decode the character Buffer passed by the server
Message decodeMessageServer(char *encodedStr) {
    //todo!!!!!!!
    Message a;
    a.state = atoi(strtok(encodedStr, DELIM));
    a.action = atoi(strtok(NULL, DELIM));
    a.arguments = strtok(NULL, DELIM);
    return a;
}
// this is to encode the message that is to be passed to the server
char* encodeMessageServer(Message m) {
    char *output;
    size_t sizeRequired = snprintf(NULL, 0, ENCODE_MESSAGE_FORMAT, m.state, m.action, m.arguments) + 1;
    output = malloc(sizeRequired * sizeof(char));
    sprintf(output, ENCODE_MESSAGE_FORMAT, m.state, m.action, m.arguments);
    return output;
}
// receiveMessage wrapper is written on-top of the recv command
// recv just receives the data from an input stream and is highly unstable with multiple send simultaneous requests
// this function uses an additional header message to read through the messages
char* receiveMessage(int socket, char *buffer) {
    char *completeMessage = NULL;
    int msgLen = 0;
    msgLen = recv(socket, buffer, BUFFER_SIZE_NETWORK, 0);
    if (msgLen < 0) {
        return completeMessage;
    }
    buffer[msgLen] = '\0';
    int v = 0;
    char headerMessage[100];
    int headerVar = 0;
    if ((buffer[0] == '<' && buffer[1] == 'h') &&
        ((buffer[2] == 'e' && buffer[3] == 'a') && (buffer[4] == 'd' && buffer[5] == '>'))) {
        // printf("HEADER Message: \n");
        v = 6;
        while ((buffer[v] != '<' && buffer[v + 1] != '\\') && ((buffer[v + 2] != 'h' && buffer[v + 3] != 'e') &&
                                                               ((buffer[v + 4] != 'a' && buffer[v + 5] != 'd') &&
                                                                buffer[v + 6] != '>'))) {
            headerMessage[headerVar++] = buffer[v++];
        }
        headerMessage[headerVar] = '\0';
        v = v + 7;
    }

    int completeMessageLen = atoi(headerMessage);
    // printf("completeMessageLen :%d\n", completeMessageLen);
    completeMessage = malloc(completeMessageLen * sizeof(char) + 1);
    memset(completeMessage, 0, completeMessageLen);
    int completeMessageVar = 0;

    while (v < msgLen) {
        completeMessage[completeMessageVar++] = buffer[v];
        v++;
    }
    while ((completeMessageLen - completeMessageVar) > 0) {
        int i = 0;
        msgLen = recv(socket, buffer, BUFFER_SIZE_NETWORK, 0);
        if (msgLen < 0)
            break;
        while (i < msgLen) {
            completeMessage[completeMessageVar] = buffer[i];
            i++;
            completeMessageVar++;
        }
    }
    completeMessage[completeMessageVar] = '\0';
    return completeMessage;
}
// this is a wrapper written on top of the send function
// adds a header message of the form "<head>messagelength</head>"
void sendMessage(int new_sock, char *serverResponse) {
    int length = strlen(serverResponse);
    char messageHeader[100];
    sprintf(messageHeader, "<head>%d</head>", length);

    int totalLength = strlen(messageHeader) + length;
    char *newMessage = malloc((totalLength + 1) * sizeof(char));
    strcpy(newMessage, messageHeader);
    strcat(newMessage, serverResponse);

    newMessage[totalLength] = '\0';
    send(new_sock, newMessage, strlen(newMessage), 0);
    // printf("@SEND: After SEND-MESSAGE:  %s\n", newMessage);
    if (strlen(newMessage) > 0)
        free(newMessage);
    return;
}
// 作为服务端的处理线程
_Noreturn void *threadedListen(void *arg) {
    char buffer[BUFFER_SIZE_NETWORK];

    int fd = *((int *) arg);

    struct sockaddr_in client;
    int clientSock;
    int sockaddrLen = sizeof(struct sockaddr_in);
    //起监听
    if (listen(fd, MAX_CLIENTS) == -1) {
        perror("listening error");
        exit(-1);
    }
    puts("[SERVER]Start Listen!!");

    while (1) {
        if ((clientSock = accept(fd, (struct sockaddr *) &client, &sockaddrLen)) == -1) {
            perror("accepting error");
            exit(-1);
        }
        puts("[SERVER]Get the Client.");

        pthread_mutex_lock(&lock);
        char *receivedMessage = receiveMessage(clientSock, buffer);
        pthread_mutex_unlock(&lock);

        if (receivedMessage != NULL) {
            // serveChunk:
            Message a = decodeMessageServer(receivedMessage);
            printf("[SERVER]server get args is:%s\n",a.arguments);
            free(receivedMessage);
            //todo:处理发过来的字段
            switch (a.action) {
                case 1:
                    a.arguments="SUCCESS";
                    pthread_mutex_lock(&lock);
                    sendMessage(clientSock,encodeMessageServer(a));
                    pthread_mutex_unlock(&lock);
                    break;
            }
        }
    }
}
