#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include <netinet/in.h>
#include<unistd.h>
#include <string.h>
//Just



using namespace std;
int Socket(int domain, int type, int protocol) {
    int res = socket(domain, type, protocol);
    if (res==-1) {
        perror("error");
        exit(EXIT_FAILURE);
    }
    return res;
}
// подключаем
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
int Accept(int sockfd,struct sockaddr *addr, socklen_t *addrlen){
    int res=accept(sockfd,addr,addrlen);
    if(res==-1){
        perror("error");
        exit(EXIT_FAILURE);
    }
    return res;
}

int main(){
    int server=Socket(AF_INET, SOCK_STREAM,0); //создаем сокет и записываем дескриптор
    struct sockaddr_in adr={0}; //  задать адрес по протоколу IPV4
    adr.sin_family=AF_INET; // семейство адресов
    adr.sin_port=htons(80); // от хоста к сети (порт на котором сервер будет слушать)
                                    // (htons- изменение порядка байт)
    adr.sin_addr.s_addr =INADDR_ANY;

    Bind(server, (struct sockaddr *) &adr, sizeof adr); // привязка сокета к адресу (server к adr)
    Listen(server,5); // слушаем на сокете server до 5 клиентов в очереди
    socklen_t adrlen=sizeof adr;  // запоминаем размер адреса
    int fd=Accept(server,(struct sockaddr *) &adr,&adrlen); // принимаем клиента и собираем в adr инфу о клиенте
    ssize_t nread; // кол-во считываемых байт
    char buf[256];
    while(true){
        memset(buf,0,256); // чистим буфер
        int Byte_recv= recv(fd, buf, 256,0); // принимаем строку от клиента
        if(Byte_recv==-1){
            cout<<"error 1"<<endl;
            break;
        }
        else if(Byte_recv==0){
            cout<<"error 2"<<endl;
            break;
        }
        cout<<"Recieved: "<<string(buf,0,Byte_recv)<<endl; // вывод от клиента
        send(fd,buf,Byte_recv+1,0); // отсылаем клиенту ответ на его сообщение
    }
    sleep(1);
    close(fd);
    close(listen(server,5));
}
//    nread=read(fd,buf,256); // максимально 256 символов (в nread реально считанное кол-во байт)
//    if (nread==-1){
//        perror("read failed");
//        exit(EXIT_FAILURE);
//    }
//    if (nread==0){
//        printf("END OF FILE\n");
//    }
//    write(STDOUT_FILENO,buf,nread);
//    write(fd,buf,nread); // на fd передаем buf и nread
//    recv(fd, buf, nread,0);
