#pragma once
// ������Ҫ ������������͹�������Ϣ
#include "BaseDef.h"


#include "CircularBuffer.h"
#include "ThreadSafeBlockQueue.h"
#include "NetPacket.h"


class ResPonseThread 
{
public:	
	~ResPonseThread(void);
	static ResPonseThread*   GetInstance(); // ��ȡ����ĵ���
	void Reset();
	int start (void * =NULL); //�������߳����������������������������ָ�롣
	void stop();     //������ֹ��ǰ�̡߳�
	void sleep (int tesec); //�����õ�ǰ�߳����߸���ʱ�䣬��λΪ�����롣
	void detach();   
	void * wait();


	bool PopPacket(INetPacket*& packet);

	inline CircularBuffer& GetReadBuffer() { return readBuffer; }

private:
	void OnRead();
private:
	ResPonseThread(void);
	pthread_t handle; 
	bool started;
	bool detached;
	static void * threadFunc(void *);
	static ResPonseThread* m_pInstance;	
	
	CircularBuffer readBuffer;						//����
	uint32		m_nRemaining;
	uint32		m_Opcode;

	ThreadSafeBlockQueue<INetPacket*>  m_msgQueue;//����

};
