//
// Created by nathan on 2/9/26.
//

#include "../../include/client_example.h"
#include "../../include/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

int retrieveAddrInfo(char address[], struct addrinfo **addrinfo_out) {
    struct addrinfo hints = {0}, *servinfo;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int rv = getaddrinfo(address, PORT, &hints, addrinfo_out);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    return 0;
}

int client_example(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    struct addrinfo *servinfo;
    int retattrinfo_err = retrieveAddrInfo(argv[1], &servinfo);
    if (retattrinfo_err != 0) {
        fprintf(stderr, "Can't retrieve addrinfo. Error code: %d\n", retattrinfo_err);
        return 1;
    }

    int sockfd;
    struct addrinfo *p;
    char ipaddrstr[INET6_ADDRSTRLEN];

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            perror("client: socket");
            continue;
        }

        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), ipaddrstr, sizeof ipaddrstr);
        printf("client: attempting connection to %s\n", ipaddrstr);

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("client: connect");
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), ipaddrstr, sizeof ipaddrstr);
    printf("client: connected to %s\n", ipaddrstr);

    freeaddrinfo(servinfo); // all done with this structure

    char buf[MAXDATASIZE];
    int numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0);
    if (numbytes == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("client: received '%s'\n",buf);

    close(sockfd);

    return 0;
}
