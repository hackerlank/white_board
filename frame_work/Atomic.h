#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#ifdef _WIN32



static inline LONG AtomicIncrement(LONG volatile * p)
{
	return (::InterlockedIncrement(p));
}
static inline LONG AtomicDecrement(LONG volatile * p)
{
	return (::InterlockedDecrement(p));
}
static inline LONG AtomicAdd(LONG volatile * p, LONG v)
{
	return (::InterlockedExchangeAdd(p, v));
}

static inline LONGLONG AtomicIncrement64(LONGLONG volatile * p)
{
	return (::InterlockedIncrement64(p));
}
static inline LONGLONG AtomicDecrement64(LONGLONG volatile * p)
{
	return (::InterlockedDecrement64(p));
}
static inline LONGLONG AtomicAdd64(LONGLONG volatile * p, LONGLONG v)
{
	return (::InterlockedExchangeAdd64(p, v));
}
#elif defined(__linux__)

#ifdef __GNUC__

static inline long AtomicIncrement(long volatile * p)
{
	return (__sync_add_and_fetch(p, 1));
}
static inline long AtomicDecrement(long volatile * p)
{
	return (__sync_sub_and_fetch(p, 1));
}
static inline long AtomicAdd(long volatile * p, long v)
{
	return (__sync_add_and_fetch(p, v));
}

static inline long long AtomicIncrement64(long long volatile * p)
{
	return (__sync_add_and_fetch(p, 1));
}
static inline long long AtomicDecrement64(long long volatile * p)
{
	return (__sync_sub_and_fetch(p, 1));
}
static inline long long AtomicAdd64(long long volatile * p, long long v)
{
	return (__sync_add_and_fetch(p, v));
}

#else
#error Unknown compiler
#endif

#elif defined(MACOSX)

#include <libkern/OSAtomic.h>
static inline long AtomicIncrement(long volatile * p)
{
	return (*p) = (*p) + 1;
	//return (OSAtomicIncrement32(p));
}
static inline long AtomicDecrement(long volatile * p)
{
	return (*p) = (*p) - 1;
	//return (OSAtomicDecrement32(p));
}
#else

#error Unknown platform

#endif //! Platform

#endif
