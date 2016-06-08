#include "BaseDef.h"

#include <errno.h>
#include "Mutex.h"
#include "assert.h"

Mutex::Mutex()
{
	int rc;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	
	if ( (rc = pthread_mutex_init(&_mutex, &attr)) != 0)
	{
		assert(false);
	}
	pthread_mutexattr_destroy(&attr);
}


Mutex::~Mutex()
{

	int rc;
	if ( (rc = pthread_mutex_destroy(&_mutex)) != 0)
	{

	}
}


int Mutex::Lock()
{
	int rc;
	if ( (rc = pthread_mutex_lock(&_mutex)) != 0)
	{
		assert(false);
		return -1;
	}
	return 0;
}


int Mutex::Unlock()
{
	int rc;
	if ( (rc = pthread_mutex_unlock(&_mutex)) != 0)
	{
		assert(false);
		return -1;
	}
	return 0;
}


int Mutex::TryLock()
{
	int rc;
	if ( (rc = pthread_mutex_trylock(&_mutex)) != 0)
	{
		if (rc == EBUSY)
		{
			return 1;
		}
		else
		{
			assert(false);
			return -1;
		}
	}
	return 0;
}


#ifdef _WIN32
#pragma warning(default: 4127)
#endif // _WIN32
