#include <sys/types.h>
#include <sys/socket.h>
#include<iostream>
#include<stdio.h>
#include <netinet/in.h>
#include<unistd.h>
#include <arpa/inet.h>
#include <string.h>
//Just a comment
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
//int Inet_pton(int af, const char *src,void *dst){
//    int res = inet_pton(af,src,dst);
//    if(res==0){
//        printf("inet_pton failed, no addres");
//        exit(EXIT_FAILURE);
//    }
//    if(res=-1){
//        perror("inet_pton failed");
//        exit(EXIT_FAILURE);
//    }
//    return res;
//}
int main(){
    int fd=Socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in adr={0};
    adr.sin_family=AF_INET;
    adr.sin_port=htons(80);
    adr.sin_addr.s_addr =INADDR_ANY;
  //  Inet_pton(AF_INET, ("127.0.0.1"),&adr.sin_addr); // стучимся в адрес (на локалку) и помещаем адрес в adr.sin_Addr
    Connect(fd, (struct sockaddr *) &adr, sizeof adr); // выполняем соединение
//    write(fd, "Hello\n",6); // пишем серверу hello
    char buf[256];
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

    close(fd);
}
