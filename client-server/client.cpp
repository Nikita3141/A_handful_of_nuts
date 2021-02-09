#include <strstream>
#include <string>
#include "message.h"
using namespace std;
int main()
{
    int cmdId, pid;
    while (getpid()<100)
        switch (pid == fork()) { // PID клиентского процесса >99
            case -1:
                perror("fork") , exit(1);
            case 0:
                break;
            default: waitpid(pid,0,0); exit (0);
        }
    cout<<"client: start executing...\n";
    message mqueue(MSGKEY); // создать объект-сообщение
    if(!mqueue.good()) exit(1); // выйти из программы, если очередь не открывается
    char procId[256], buf2[256];
    ostrstream(procId,sizeof (procId)) << getpid() << '\0';
    do {
        cout<< "cmd> " << flush; // вывести на экран приглашение на ввод
        cin >> cmdId; //прочитать сообщение пользователя
        cout<< endl;
        if(cin.eof()) break; // выйти, если встретился символ EOF
        // проверить допутимость команды
        if ((!cin.good()) || ( cmdId<0) || ( cmdId >99)) {
            cerr <<"Invalid input:"<<cmdId<<endl; continue;
}
        // передать запрос в демон
        if(mqueue.send(procId,strlen(procId),cmdId))
            cout<<"Client:"<< getpid() << "msgsnd error\n";
        else if(cmdId==QUIT_CMD) break; // при QUIT_CMD выйти
        // принять данные от демона
        else if(mqueue.rcv(buf2,sizeof(buf2) ,getpid(),0)==-1)
            cout <<"client:"<<getpid()<< "msgrv error\n";
        // вывести на экран ответ сервера
        else cout <<"client:"<< getpid() <<"msgrcv error \n";
    } while(true); // цикл до EOF
    cout<<"client:" <<getpid() <<"exiting...\n"<<flush;
    return 0;
}