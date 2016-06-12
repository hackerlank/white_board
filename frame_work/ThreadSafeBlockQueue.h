#pragma once
#include "Mutex.h"

template < typename T, typename Sequence = std::deque<T> >
class ThreadSafeBlockQueue
{
	public:
		bool IsEmpty() const	{ return _c0.empty() && _c1.empty(); }
		size_t Size() const		{ return _c0.size() + _c1.size(); }
		size_t SizeH() const	{ return _c1.size(); }
		size_t SizeL() const	{ return _c0.size(); }

		ThreadSafeBlockQueue()
		{
			int rc;
			if ( (rc = pthread_cond_init(&_is_empty, NULL)) != 0)
			{
				fprintf(stderr, "pthread_cond_init() failed: %d\n", rc);
			}
		}

		~ThreadSafeBlockQueue()
		{
			pthread_cond_destroy(&_is_empty);
		}	

		void Enqueue(const T &x,  uint32 priority = 0)
		{
			_mutex.Lock();
			if (priority == 0)
				_c0.push_back(x);
			else
				_c1.push_back(x);
			int rc;
			if ( (rc = pthread_cond_signal(&_is_empty)) != 0)
			{
				fprintf(stderr, "pthread_cond_signal() failed: %d\n", rc);
				//abort();
			}
			_mutex.Unlock();
		}
		int Dequeue(T &x, long timeout_ms)
		{
			_mutex.Lock();
			while (IsEmpty())
			{
				struct timespec ts;
				ts.tv_sec = (long)time(NULL) + timeout_ms/1000;
				ts.tv_nsec = (timeout_ms%1000) * (1000*1000);

				int rc;
				if ( (rc = pthread_cond_timedwait(&_is_empty, _mutex.Get(), &ts)) != 0)
				{                    
                    // Both errorno.h and pthread.h have ETIMEDOUT defined.
                    // Use literal ETIMEDOUT, to avoid include conflict.
					if (rc == 10060 /* ETIMEDOUT in pthread.h */ 
                        || rc == 138 /* ETIMEDOUT in errorno.h */
                        )
					{
						_mutex.Unlock();
						return 1; // time out
					}
					else
					{
						fprintf(stderr, "pthread_cond_wait() failed: %d\n", rc);
						//abort();
						_mutex.Unlock();
						return -1; // error
					}
				}
			}
			if (!_c1.empty())
			{
				x = _c1.front();
				_c1.pop_front();
			}
			else
			{
				x = _c0.front();
				_c0.pop_front();
			}
			_mutex.Unlock();
			return 0;
		}

	private:
		Sequence _c0, _c1;
		Mutex _mutex;
		pthread_cond_t _is_empty;
};
