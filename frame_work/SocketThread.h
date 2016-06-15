#pragma once

#include <atomic> 
#include "ODSocket.h"
#include "BaseDef.h"


class SocketThread
{
public:	
	~SocketThread(void);
	static SocketThread*   GetInstance();
	int start();  
	ODSocket getSocket();
	std::atomic_long  state;// 0 ��ʾ���ӳɹ� 1 ��ʾ����ʧ��
	ODSocket csocket;	
	void stop();//������ֹ��ǰ�̡߳�
	pthread_t getThreadId();
	bool connectOk();
private:
	pthread_t pid;	
	static void* start_thread(void *); 	
	SocketThread(void);
private:
	static SocketThread* m_pInstance;	
};

