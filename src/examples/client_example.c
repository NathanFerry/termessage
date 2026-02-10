//
// Created by nathan on 2/9/26.
//

#include <errno.h>
#include <netdb.h>
#include <stdio.h>

void client_example() {
    struct addrinfo hints = {0}, *res;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    //hints.ai_flags = AI_PASSIVE; For localhost, use this flag and set name in getaddrinfo to NULL

    getaddrinfo("www.example.com", "3490", &hints, &res);

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    int connect_err = connect(sockfd, res->ai_addr, res->ai_addrlen);
    if (connect_err == -1) {
        fprintf(stderr, "Connect errored with errno: %d\n", errno);
    }

    printf("QUIT\n");
}
