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

// decode the message sent by Server and call the respective functions to get the desired functionality
// in simple terms, decode the character Buffer passed by the server
Message decodeMessage(char *encodedStr) {
    //todo!!!!!!!
    Message a;
    a.state = atoi(strtok(encodedStr, DELIM));
    a.action = atoi(strtok(NULL, DELIM));
    a.command = strtok(NULL, DELIM);
    return a;
}
// this is to encode the message that is to be passed to the server
char* encodeMessage(Message m) {
    char *output;
    size_t sizeRequired = snprintf(NULL, 0, MESSAGE_FORMAT, m.state, m.action, m.command) + 1;
    output = malloc(sizeRequired * sizeof(char));
    sprintf(output, MESSAGE_FORMAT, m.state, m.action, m.command);
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
