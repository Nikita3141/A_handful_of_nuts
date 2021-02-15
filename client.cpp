#include <sys/socket.h>
#include<iostream>
#include<stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
using namespace std;
void Connect(int sockfd,const struct sockaddr *addr,socklen_t addrlen){
    int res=connect(sockfd,addr,addrlen);
    if(res==-1){
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
}
int Socket(int domain, int type, int protocol) {
    int res = socket(domain, type, protocol);
    if (res==-1) {
        perror("error");
        exit(EXIT_FAILURE);
    }
    return res;
}
int main(){
    int fd=Socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in adr={0};
    adr.sin_family=AF_INET;
    adr.sin_port=htons(80);
    //    adr.sin_addr.s_addr =INADDR_ANY; // люой
    // inet_pton(AF_INET,"192.168.1.101",&adr.sin_addr); // на случай явного задания адреса
    Connect(fd, (struct sockaddr *) &adr, sizeof adr); // выполняем соединение
    char buf[1024];
    string user_input; // ввод пользователя
    do{
        cout<<">";
        getline(cin,user_input); // считываем сообщение пользователя
        if(user_input=="!exit") break; // условие выхода
        int sendRes=send(fd, user_input.c_str(),user_input.size()+1,0); // отправляем сообщение на сервер
        if(sendRes==-1){
            cout<<"Error 4"<<endl;
            continue;
        }
        memset(buf,0,256); // чистим буфер
        int Byte_recv=recv(fd,buf,256,0); // принимаем ответ от сервера
        cout<<"Server: "<<string(buf,Byte_recv)<<endl; // выводим ответ от сервера
    } while (true);
    return 0;
}