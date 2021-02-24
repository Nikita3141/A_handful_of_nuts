#include<iostream>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<thread>

class Server {
public:
    Server();
    bool Startserver();

private:
    int server;
    char buf[1024];
    sockaddr_in adr{};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int Socket(int domain, int type, int protocol);
    void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    void Listen(int sockfd, int backlog);
    int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    void threadFunction();
};
