#pragma once

//#define PTW32_STATIC_LIB
#include <pthread.h>

class Mutex
{
	friend class Condition;
public:
	Mutex();
	virtual ~Mutex();

	int Lock();
	int Unlock();
	int TryLock();

	void Acquire(){ Lock(); }
	void Release(){ Unlock(); }
	bool AttemptAcquire(){ return TryLock()==0; }

	pthread_mutex_t* Get() { return &_mutex; }

private:
	pthread_mutex_t _mutex;
};


template<class LOCK>
class _CAutoLock
{	
	LOCK* m_pLock;
public:
	_CAutoLock(LOCK* pLock )
	{
		m_pLock = pLock;
		m_pLock->Acquire();
	}

	~_CAutoLock()
	{
		m_pLock->Release();
	}
};
#define CAutoLock _CAutoLock<Mutex>
#define CAutoRwLock _CAutoLock<RwLock>
