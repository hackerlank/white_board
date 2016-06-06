
#include "SocketThread.h"

void main(){
	SocketThread* th = SocketThread::GetInstance();
	th->start();
	pthread_join(th->getThreadId(), nullptr);
}