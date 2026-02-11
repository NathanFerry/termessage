//
// Created by Nathan on 1/27/26.
//

#include "../include/client_example.h"
#include "../include/server_example.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        server_example();
    } else {
        client_example(argc, argv);
    }
}
