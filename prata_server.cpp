#include <ststream.h>
#include "message.h"
#include <string.h>
#include <signal.h>
#include <ctime>

int main()

{
	int 	len, pid, cmdId;
	char	buf[256];
	time_t	tim;

	//Настроить этот процесс как демон
	for(int i = 0; i < 20;i++) signal(i,SIG_IGN);
	setsid();
	cout<<"server: start executing...\n" << flush;
	message mqueue (MSGKEY);
	if(!mqueue.good()) exit(1);

	//Ожидать запрос от клиента
	while(len = mqueue.rcv(buf,sizeof(buf),-99,&cmdId) > 0){
		//Получить идентификатор процесса клиента и проверить его достоверность
		istrstream(buf,sizeof(buf))>>pid;
		if(pid < 100){
			cerr << "Illegal PID: " << buf << " " << pid << endl;
			continue;
		}
		//Подготовить ответ клиенту
		cerr << "server: recieve cmd #" << cmdId << "from client:"<<pid<<endl;
		switch(cmdId){

			case LOCAL_TIME:
				tim = time(0);
				strcpy(buf,ctime(&tim));
				break;
			case UTC_TIME:
				tim = time(0);
				strcpy(buf,asctime(gmtime(&tim)));
				break;
			case QUIT_CMD:
				cerr << "server: deleting msg queue ...\n";
				return mqueue.rmQ();
			default:
				ostrstream(buf,sizeof(buf))<<"Illegal cmd:" << cmdId << '\0;
		}
		//Послать клиенту ответ
		if(mqueue.send(buf,strlen(buf)+1,pid)==-1)
		cerr << "Server: "<<getpid() << "send response fails\n";
	} // Бесконечный цикл
	return 0;
}
