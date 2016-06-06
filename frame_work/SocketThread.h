#pragma once
#include "ODSocket.h"
#include "pthread.h"
class SocketThread
{
public:	
	~SocketThread(void);
	static SocketThread*   GetInstance();
	int start();  
	ODSocket getSocket();
	int state;// 0 ��ʾ���ӳɹ� 1 ��ʾ����ʧ��
	ODSocket csocket;	
	void stop();//������ֹ��ǰ�̡߳�
	pthread_t getThreadId();
private:
	pthread_t pid;	
	static void* start_thread(void *);//��̬��Ա����,�൱��C�е�ȫ�ֺ��� 	
	SocketThread(void);
private:
	static SocketThread* m_pInstance;	
};

