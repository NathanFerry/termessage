//
// Created by nathan on 2/9/26.
//

#include <errno.h>
#include <netdb.h>
#include <stdio.h>

#define MYPORT "3490"
#define BACKLOG 10

void server_example() {
    // Preparation of the res addrinfo struct
    struct addrinfo hints = {0}, *res;

    struct sockaddr_storage their_addr;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // For localhost, use this flag and set name in getaddrinfo to NULL

    getaddrinfo(NULL, MYPORT, &hints, &res);

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    int bind_err = bind(sockfd, res->ai_addr, res->ai_addrlen);
    if (bind_err == -1) {
        fprintf(stderr,"Bind errored with errno: %d\n", errno);
    }

    int listen_err = listen(sockfd, BACKLOG);
    if (listen_err == -1) {
        fprintf(stderr, "Listen errored with errno: %d\n", errno);
    }

    socklen_t addr_size = sizeof their_addr;
    int new_fd = accept(sockfd, (struct sockaddr *) &their_addr,  &addr_size);

    printf("QUIT\n");
    freeaddrinfo(res);
}
