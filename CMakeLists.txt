cmake_minimum_required(VERSION 3.17)
project(client_server)

set(CMAKE_CXX_STANDARD 14)

add_executable(client_server server.cpp message.h server.cpp client.cpp draft.cpp server_draft.cpp)