#include "Server.h"

Server::Server() {
    adr = {0}; //  задать адрес по протоколу IPV4
    adr.sin_family = AF_INET; // семейство адресов
    adr.sin_port = htons(80); // от хоста к сети (порт на котором сервер будет слушать)
    // (htons- изменение порядка байт)
    adr.sin_addr.s_addr = INADDR_ANY;
    server = Socket(AF_INET, SOCK_STREAM, 0); //создаем сокет и записываем дескриптор
    Bind(server, (struct sockaddr *) &adr, sizeof adr); // привязка сокета к адресу
}

int Server::Socket(int domain, int type, int protocol) {
    int res = socket(domain, type, protocol);
    if (res==-1) {
        perror("error");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Server::Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int res=bind(sockfd, addr, addrlen);
    if (res==-1) {
        perror("error");
        exit(EXIT_FAILURE);
    }

}

void Server::Listen(int sockfd, int backlog) {
    int res=listen(sockfd,backlog);
    if (res == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

int Server::Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int res=accept(sockfd,addr,addrlen);
    if(res==-1){
        perror("error");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Server::threadFunction() {
    Listen(server,1); // слушаем на сокете server
    socklen_t adrlen=sizeof adr;  // запоминаем размер адреса
    int fd=Accept(server,(struct sockaddr *) &adr,&adrlen); // принимаем клиента и собираем в adr инфу о клиенте
    while(true) {
        memset(buf, 0, 1024); // чистим буфер
        int Byte_recv = recv(fd, buf, 1024, 0); // принимаем строку от клиента
        if (Byte_recv == -1) {
            std::cout << "error 1" << std::endl;
            break;
        } else if (Byte_recv == 0) {
            std::cout << "error 2" << std::endl;
            break;
        }
        std::cout << "Recieved: " << std::string(buf, 0, Byte_recv) << std::endl; // вывод от клиента
        send(fd, buf, Byte_recv + 1, 0); // отсылаем клиенту ответ на его сообщение
    }
    sleep(1);
    close(fd);
    close(listen(server,1));
}

bool Server::Startserver() {
    std::thread thread1(&Server::threadFunction, this);
    std::thread thread2(&Server::threadFunction, this);
    thread1.join();
    thread2.join();
    return true;
}
