#pragma once
// ������Ҫ ������������͹�������Ϣ
#include "BaseDef.h"


#include "CircularBuffer.h"
#include "ThreadSafeBlockQueue.h"
#include "NetPacket.h"

/*
typedef void (cocos2d::CCObject::*ResPonseThreadEvent)(BaseResponseMsg*);
#define callFunc_selectormsg(_SELECTOR) (ResPonseThreadEvent)(&_SELECTOR)

#define M_ADDCALLBACKEVENT(varName)\
protected: cocos2d::CCObject* m_##varName##listener;ResPonseThreadEvent varName##selector;\
public: void add##varName##ListenerEvent(ResPonseThreadEvent m_event,cocos2d::CCObject* listener)  { m_##varName##listener=listener;varName##selector=m_event; }
*/
class ResPonseThread 
{
public:	
	~ResPonseThread(void);
	static ResPonseThread*   GetInstance(); // ��ȡ����ĵ���
	void Reset();
	int start (void * =NULL); //�������߳����������������������������ָ�롣
	void stop();     //������ֹ��ǰ�̡߳�
	void sleep (int tesec); //�����õ�ǰ�߳����߸���ʱ�䣬��λΪ�����롣
	void detach();   //
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
	//M_ADDCALLBACKEVENT(msg);// ����ص�����
	//M_ADDCALLBACKEVENT(notcon);//�����ص�����
	
	CircularBuffer readBuffer;						//����
	uint32		m_nRemaining;
	uint32		m_Opcode;

	ThreadSafeBlockQueue<INetPacket*>  m_msgQueue;//����

};
