//
// Created by nathan on 2/9/26.
//

#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int find_ip_of_domain_name(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "To many or to few arguments");
        return 1;
    }

    int status;
    struct addrinfo hints = {0}, *servinfo;
    char ipstr[INET6_ADDRSTRLEN];

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], "3490", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    }

    printf("IP addresses for %s\n\n", argv[1]);

    for (struct addrinfo *p = servinfo; p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;

        // You can cast sockaddr to sockaddr_in(6) and vice-versa
        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf(" %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(servinfo);

    return 0;
}
