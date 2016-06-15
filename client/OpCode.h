#ifndef _OP_CODE_
#define _OP_CODE_

/*
该枚举是通信协议交互时,客户端与服务器约定的协议号
*/
enum Opcode
{
	eLogin=100,  //登录
	eEnterRoom,//进房间
	eAddShape,//增加或修改图元
	eRemoveShape,//删除图元
	eErrorCode,//错误消息

	
};

#endif // _OP_CODE_