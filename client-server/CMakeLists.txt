cmake_minimum_required(VERSION 3.17)
project(client_server)

set(CMAKE_CXX_STANDARD 14)

add_executable(client_server client.cpp message.h)