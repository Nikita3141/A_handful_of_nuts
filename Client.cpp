#include "Client.h"

Client::Client() {
    fd = socket(AF_INET,SOCK_STREAM,0);
    adr={0};
    adr.sin_family=AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&adr.sin_addr);
    adr.sin_port=htons(80);
}


bool Client::ClientStart() {
    int res = connect(fd, (struct sockaddr *) &adr, sizeof adr);{
        if(res==-1) {
            perror("Error");
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

int Client::Handle(){
    do{
        std::cout<<">";
        getline(std::cin,user_input); // считываем сообщение пользователя
        if(user_input=="!exit") break; // условие выхода
        int sendRes = send(fd, user_input.c_str(),user_input.size()+1,0); // отправляем сообщение на сервер
        if(sendRes==-1){
            std::cout<<"Error 4"<<std::endl;
            continue;
        }
        memset(buf,0,1024); // чистим буфер
        int Byte_recv=recv(fd,buf,1024,0); // принимаем ответ от сервера
        std::cout<<"Server: "<<std::string(buf,Byte_recv)<<std::endl; // выводим ответ от сервера
    } while (true);
    return 0;
}

void Client::ClientStop() {
    delete[] buf;
    close(fd);
}

void Client::Run(){
    ClientStart();
    Handle();
    ClientStop();
}




