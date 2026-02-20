//
// Created by nathan on 2/19/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <stdbool.h> // C23: bool is now a keyword, but header is good practice

#define PORT 8080
#define MAX_FDS 64

int server_and_client(int argc, char* argv[]) {
    struct pollfd fds[MAX_FDS];
    int nfds = 0;

    // 1. Create the Listening (Server) Socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };

    bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listen_fd, 5);

    // Add Stdin and Listener to poll
    fds[0] = (struct pollfd){ .fd = STDIN_FILENO, .events = POLLIN };
    fds[1] = (struct pollfd){ .fd = listen_fd, .events = POLLIN };
    nfds = 2;

    // 2. THE CLIENT PART: Connect to a peer if an IP is provided
    if (argc > 1) {
        int client_fd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in peer_addr = {
            .sin_family = AF_INET,
            .sin_port = htons(PORT)
        };
        inet_pton(AF_INET, argv[1], &peer_addr.sin_addr);

        printf("Connecting to %s...\n", argv[1]);
        if (connect(client_fd, (struct sockaddr*)&peer_addr, sizeof(peer_addr)) == 0) {
            fds[nfds++] = (struct pollfd){ .fd = client_fd, .events = POLLIN };
            printf("Connected to peer successfully!\n");
        } else {
            perror("Connection failed");
        }
    }

    // 3. The Unified Event Loop
    while (true) {
        if (poll(fds, nfds, -1) < 0) break;

        for (int i = 0; i < nfds; i++) {
            if (!(fds[i].revents & POLLIN)) continue;

            // Incoming connection to our Server
            if (fds[i].fd == listen_fd) {
                int new_peer = accept(listen_fd, nullptr, nullptr);
                if (nfds < MAX_FDS) {
                    fds[nfds++] = (struct pollfd){ .fd = new_peer, .events = POLLIN };
                    printf("[System] New peer joined the chat.\n");
                }
            }
            // We typed something (Client Outbound)
            else if (fds[i].fd == STDIN_FILENO) {
                char msg[1024];
                if (fgets(msg, sizeof(msg), stdin)) {
                    // Send to EVERYONE except stdin and the listener
                    for (int j = 2; j < nfds; j++) {
                        send(fds[j].fd, msg, strlen(msg), 0);
                    }
                }
            }
            // Data received from any Peer
            else {
                char buf[1024] = {0};
                ssize_t bytes = recv(fds[i].fd, buf, sizeof(buf) - 1, 0);
                if (bytes <= 0) {
                    printf("[System] A peer disconnected.\n");
                    close(fds[i].fd);
                    fds[i] = fds[--nfds]; // Swap with last and decrement
                    i--; // Re-check this index since it's a new FD now
                } else {
                    printf("> %s", buf);
                }
            }
        }
    }
    return 0;
}