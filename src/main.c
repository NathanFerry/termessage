//
// Created by Nathan on 1/27/26.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int main() {
    struct addrinfo hints = {0}, *servinfo;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
}
