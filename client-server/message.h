#ifndef MESSAGE_H
#define MMESSAGE_H
#include <iostream>
#include <memory.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
// общие объявления для серверного процесса
enum {MSGKEY=176, MAX_LEN=256, ILLEGAL_CMD=4};
enum {LOCAL_TIME=1, UTC_TIME=2,QUIT_CMD=3};
struct mgbuf
{
    long mtype;
    char mtext[MAX_LEN];
};
msgbuf MSGBUF;// объект структуры
class message {
private:
    int msgId; // дескриптор очереди сообщений
    mgbuf mObj;
public:
    // конструктор. Получаем указатель на очередь сообщений
    message(int key)
    {
        if(msgId=msgget(key,IPC_CREAT|0666)==-1)
            perror("msgget");
    };
    ~message(); // ничего не делает
    // проверить открыта ли очередь сообщений
    int good() {return (msgId>=0) ? 1: 0;};
    //удалить очередь сообщений
    int rmQ()
    {
        int rc=msgctl(msgId,IPC_RMID,0);
        if (rc==-1) perror("msgctl");
        return rc;
    }
    // передать сообщение
    int send (const void* buf, int size,int type)
    {
        mObj.mtype=type;
        memcpy(mObj.mtext,buf,size);
        if(msgsnd(msgId,&mObj,size,0)){
            perror("msgsnd");
            return -1;
        }
    }
    // принять сообщение
    int rcv (void * buf,int size, int type, int *rtype){
        int len=msgrcv(msgId,&mObj,MAX_LEN,type, MSG_NOERROR)
                if(len==-1){
                    perror("msgrcv");
                    return -1;
                }
                memcpy(buf,mObj.mtext,len);
                if ((rtype) * rtype =mObj.mtype) return len;
    }

};
#endif