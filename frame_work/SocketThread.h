#pragma once
#include "ODSocket.h"
#include "BaseDef.h"

class SocketThread
{
public:	
	~SocketThread(void);
	static SocketThread*   GetInstance();
	int start();  
	ODSocket getSocket();
	int state;// 0 表示连接成功 1 表示连接失败
	ODSocket csocket;	
	void stop();//函数中止当前线程。
	pthread_t getThreadId();
private:
	pthread_t pid;	
	static void* start_thread(void *); 	
	SocketThread(void);
private:
	static SocketThread* m_pInstance;	
};

