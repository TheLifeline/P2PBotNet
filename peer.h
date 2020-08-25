//
// Created by user on 2020/8/4.
//

#ifndef P2PBOTNET_PEER_H
#define P2PBOTNET_PEER_H


#include <stdbool.h>
#include <stdint.h>
#include "global.h"

#define BUFFER_SIZE_NETWORK 512
#define MAX_CLIENTS 10

typedef struct Message {
    int state;
    int action;
    char *arguments;
    char *file;
} Message;
typedef struct
{
    uint32_t ServantIP;
    uint16_t ServantPort;
    char *BotID;
    uint32_t FailCount;
} Peer;
Peer peer_list[10];//维护一个peer_list
char *encodeMessageServer(Message);

Message decodeMessageServer(char *encodedStr);

void sendMessage(int, char *);

char *receiveMessage(int, char *);

_Noreturn void *threadedListen(void *);

int registerPeer(int serverSock, int listeningPort);

#endif //P2PBOTNET_PEER_H
