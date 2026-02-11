//
// Created by nathan on 2/11/26.
//

#ifndef TERMESSAGE_UTILS_H
#define TERMESSAGE_UTILS_H

#include <sys/socket.h>

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr *sa);

#endif //TERMESSAGE_UTILS_H