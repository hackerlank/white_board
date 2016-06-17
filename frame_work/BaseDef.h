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

#define CLIENTSOCKET_RECVBUF_SIZE 6000000



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


#if _BIG_ENDIAN_
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

#define ERROR_NOT_IN_ROOM ("not in any room")
#define ERROR_PACKET_WRONG ("the packet you send is wrong")
#define ERROR_SHAPE_ID_NOT_EXIST ("this shape id is not exist")
#define ERROR_ADD_SHAPE_TOO_MANY ("Opertion error,you add too many shape once")
#define ERROR_CREATE_ROOM_EXIST  ("create room when room already exist")
#define ERROR_ENTER_ROOM_NOT_EXIST ("enter room when room not exist")
#define ERROR_COPY_ROOM_SRC_NOT_EXIST ("copy room src room not exist")
#define ERROR_COPY_ROOM_DES_EXIST ("copy room des room already exist")
#define ERROR_SYNC_PLAYER_LIST_NO_ROOOM ("ask player list ,room not exist")
#define ERROR_DELETE_ROOM_NOT_EXIST ("delete room not exist")
#define ERROR_ROOM_DELET ("the room you in has been delete,you quit")

enum ShapeErrorCode
{
	Shape_Common_Error = 0,	//通常错误
	Shape_No_In_Room,//不在任何桌子内操作 图元
	Shape_Packet_Wrong,//发送的协议内容有问题
	Shape_Id_Not_Exist,//试图删除的图元不存在
	Shape_Wrong_Operation,//错误操作
	Shape_Leave_When_not_In_Room,//离桌但不在房间里
	Shape_Create_Room_When_Key_Exist,//创建房间但房间key已存在
	Shape_Copy_Room_When_Src_Key_No_Exist,//拷贝房间 原始房间不存在
	Shape_Copy_Room_When_Des_Key_Exist,//拷贝房间 目的房间已存在
	Shape_Enter_Room_Not_Exist,//进入的房间不存在
	Shape_Sync_Player_List_No_Room, //请求的房间的人列表,但是房间不存在
	Shape_Delete_Room_Not_Exist,//删除的房间不存在
	Shape_Room_Delete_You_Out,//房间被删除,你被剔除房间
};

/*
oprate_type
1 请求房间列表
2 创建房间
3 进入房间
4 离开房间
5 销毁房间
6 拷贝房间
7 请求房间内玩家列表
*/
enum RoomOperateType
{
	RoomOperate_Ask_Room_List = 1,
	RoomOperate_Create_Room,
	RoomOperate_Enter_Room,
	RoomOperate_Leave_Room,
	RoomOperate_Delete_Room,
	RoomOperate_Copy_Room,
	RoomOperate_Ask_Player_List,
};


#define USR_NAME "2222"
#define ROOM_NAME "test_room_src"
#define ROOM_COPY_TO "test_room_des"