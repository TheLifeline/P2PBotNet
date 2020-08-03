//
// Created by jiemin on 2020/8/3.
//

#include <stdint.h>
#include <stdlib.h>
#include "tool.h"
#include <arpa/inet.h>
#define INET_ADDR(o1, o2, o3, o4) (htonl((o1 << 24) | (o2 << 16) | (o3 << 8) | (o4 << 0)))

int checkipstr(char const *ipstr, char ip[4][4]) {
    if (ipstr == NULL) {
        return -1;
    }
    int i = 0;
    int j = 0;
    for (; *ipstr != '\0'; ipstr++) { //skip the space in every round
        if (*ipstr == ' ') {
            continue;
        }
        //store the data, if it is not '\0','.',and space
        while (*ipstr != '.' && *ipstr != ' ' && *ipstr != '\0') {
            ip[i][j] = *ipstr;
            // printf("%s\n",ipstr);
            j++;
            ipstr++;
        }
        if (*ipstr == '.') {
            ip[i][j] = '\0';
            i++;
            j = 0;
            continue;
        } else if (*ipstr == " ") {
            while (*ipstr == ' ')
                ipstr++;
            if (*ipstr != '.' && *ipstr != '\0')
                return -2;
            else {
                ip[i][j] = '\0';
                i++;
                j = 0;
            }
        } else if (*ipstr == '\0') {
            ip[i][j] = '\0';
            break;
        }
    }
    return 0;
}

Input *check_input(int argc, char const *argv[]) {
    char ip[4][4] = {'0'};
    if (argc == 3 && checkipstr(argv[1], ip) == 0) {
        Input *user_input = (Input *) malloc(sizeof(Input));
        user_input->IP = INET_ADDR(atoi(ip[0]), atoi(ip[1]), atoi(ip[2]), atoi(ip[3]));
        user_input->Port = (uint16_t)atoi(argv[2]);
        return user_input;
    }
    return NULL;
}