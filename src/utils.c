//
// Created by nathan on 2/11/26.
//

#include "../include/utils.h"

#include <sys/socket.h>
#include <arpa/inet.h>

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}