#pragma once
// 此类主要 处理服务器推送过来的消息
#include "BaseDef.h"


#include "CircularBuffer.h"
#include "ThreadSafeBlockQueue.h"
#include "NetPacket.h"


class ResPonseThread 
{
public:	
	~ResPonseThread(void);
	static ResPonseThread*   GetInstance(); // 获取该类的单利
	void Reset();
	int start (void * =NULL); //函数是线程启动函数，其输入参数是无类型指针。
	void stop();     //函数中止当前线程。
	void sleep (int tesec); //函数让当前线程休眠给定时间，单位为毫秒秒。
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
	
	CircularBuffer readBuffer;						//无锁
	uint32		m_nRemaining;
	uint32		m_Opcode;

	ThreadSafeBlockQueue<INetPacket*>  m_msgQueue;//有锁

};
