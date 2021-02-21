#include <sys/socket.h>
#include<iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
//client class
using namespace std;
class Client{
public:
    bool ClientStart();
    void ClientStop();
    void Handle();
    Client();
private:
    int fd;
    char* buf = new char[1024];
    string user_input;
    sockaddr_in adr;
};


//bool Client::ClientStart() {
//    connect(fd, (struct sockaddr *) &adr, sizeof adr);
//    return true;
//}

void Client::Handle(){
    do{
        cout<<">";
        getline(cin,user_input); // считываем сообщение пользователя
        if(user_input=="!exit") break; // условие выхода
        int sendRes = send(fd, user_input.c_str(),user_input.size()+1,0); // отправляем сообщение на сервер
        if(sendRes==-1){
            cout<<"Error 4"<<endl;
            continue;
        }
        memset(buf,0,1024); // чистим буфер
        int Byte_recv=recv(fd,buf,1024,0); // принимаем ответ от сервера
        cout<<"Server: "<<string(buf,Byte_recv)<<endl; // выводим ответ от сервера
    } while (true);
}

Client::Client() {
    fd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in adr={0};
    adr.sin_family=AF_INET;
    inet_pton(AF_INET,"192.168.1.105",&adr.sin_addr);
    adr.sin_port=htons(80);
    connect(fd, (struct sockaddr *) &adr, sizeof adr);
}


void Client::ClientStop() {
    delete[] buf;
    close(fd);
}

int main(){
    Client c1;
//    c1.ClientStart();
    c1.Handle();
    c1.ClientStop();
}