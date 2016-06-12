#pragma once

#include "stdint.h"
#include <stdio.h> 
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <deque>
#include <string>
#include <limits>
#include <set>
#include <string>
#include <process.h>
#include <assert.h>
#include "time.h"

#include <Windows.h>
#include "pthread.h"

#define CLIENTSOCKET_RECVBUF_SIZE 64000



typedef  unsigned char	BYTE;
typedef  unsigned int	UINT;

typedef signed __int64 int64;
typedef signed __int32 int32;
typedef signed __int16 int16;
typedef signed __int8 int8;
typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;


#if !_LITTLE_ENDIAN_
#define _BITSWAP16(x) (((x << 8) & 0xff00) | ((x >> 8) & 0x00ff))

#define _BITSWAP32(x) (((x << 24) & 0xff000000) | ((x <<  8) & 0x00ff0000) | \
	((x >>  8) & 0x0000ff00) | ((x >> 24) & 0x000000ff))

#define _BITSWAP64(x) (((x << 56) & 0xff00000000000000ULL) | ((x << 40) & 0x00ff000000000000ULL) | \
	((x << 24) & 0x0000ff0000000000ULL) | ((x <<  8) & 0x000000ff00000000ULL) | \
	((x >>  8) & 0x00000000ff000000ULL) | ((x >> 24) & 0x0000000000ff0000ULL) | \
	((x >> 40) & 0x000000000000ff00ULL) | ((x >> 56) & 0x00000000000000ffULL))
#else
#define _BITSWAP16(x) (x)
#define _BITSWAP32(x) (x)
#define _BITSWAP64(x) (x)
#endif


enum ErrorCode
{
	ECommonSuccess=0,  //成功
	ECommonError,//失败
	EErrorNoHandlerFun,//没有对应的处理函数
};