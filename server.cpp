#include<iostream>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<thread>
using namespace std;

class serv{
public:
    serv();
    bool Startserver();
    void threadFunction();
private:
    int server;
    char buf[1024];
    struct sockaddr_in adr{};
    int Socket(int domain, int type, int protocol) {
        int res = socket(domain, type, protocol);
        if (res==-1) {
            perror("error");
            exit(EXIT_FAILURE);
        }
        return res;
    }
    void Bind(int sockfd,const struct sockaddr *addr,socklen_t addrlen){
        int res=bind(sockfd, addr, addrlen);
        if (res==-1) {
            perror("error");
            exit(EXIT_FAILURE);
        }
    }
    void Listen(int sockfd,int backlog) {
        int res=listen(sockfd,backlog);
        if (res == -1) {
            perror("listen failed");
            exit(EXIT_FAILURE);
        }
    }
    int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
        int res=accept(sockfd,addr,addrlen);
        if(res==-1){
            perror("error");
            exit(EXIT_FAILURE);
        }
        return res;
    }
};
void serv::threadFunction(){
    Listen(server,1); // слушаем на сокете server
    socklen_t adrlen=sizeof adr;  // запоминаем размер адреса
    int fd=Accept(server,(struct sockaddr *) &adr,&adrlen); // принимаем клиента и собираем в adr инфу о клиенте
    while(true) {
        memset(buf, 0, 1024); // чистим буфер
        int Byte_recv = recv(fd, buf, 1024, 0); // принимаем строку от клиента
        if (Byte_recv == -1) {
            cout << "error 1" << endl;
            break;
        } else if (Byte_recv == 0) {
            cout << "error 2" << endl;
            break;
        }
        cout << "Recieved: " << string(buf, 0, Byte_recv) << endl; // вывод от клиента
        send(fd, buf, Byte_recv + 1, 0); // отсылаем клиенту ответ на его сообщение
    }
    sleep(1);
    close(fd);
    close(listen(server,1));
}
serv::serv() {
    adr = {0}; //  задать адрес по протоколу IPV4
    adr.sin_family = AF_INET; // семейство адресов
    adr.sin_port = htons(80); // от хоста к сети (порт на котором сервер будет слушать)
    // (htons- изменение порядка байт)
    adr.sin_addr.s_addr = INADDR_ANY;
    server = Socket(AF_INET, SOCK_STREAM, 0); //создаем сокет и записываем дескриптор
    Bind(server, (struct sockaddr *) &adr, sizeof adr); // привязка сокета к адресу (server к adr)
}
bool serv::Startserver() {
    std::thread thread1(&serv::threadFunction, this);
    std::thread thread2(&serv::threadFunction, this);
    thread1.join();
    thread2.join();
    return true;
}
int main() {
    serv server;
    server.Startserver();
    return 0;
}