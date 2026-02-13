cmake_minimum_required(VERSION 3.28)
project(termessage C)

set(CMAKE_C_STANDARD 23)

add_executable(termessage)

target_sources(termessage
        PRIVATE
            src/main.c
            src/examples/utils.c
            src/examples/client_example.c
            src/examples/server_example.c
)