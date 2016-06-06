#pragma once
//#include "chat_msg_req.pb.h"
/****
 这个主要是把 从请求中得到的值  转化成 所需要的对象
*****/

#define M_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public:  varType get##funName(void) const { return varName; }\
public:  void setStringTo##funName(const char* str,const int myl){ varType baseresponse; baseresponse.ParseFromArray(str,myl);varName=baseresponse;}

class BaseResponseMsg
{
  
public:
	BaseResponseMsg(void);
	~BaseResponseMsg(void);
	short code;// 响应编号
	int playerId; // 用户编号
	int state;// 这个是 当前的状态  0 表示 连接成功 1 表示连接断开
	short len;// 数据长度
	//M_SYNTHESIZE(zzboy::protobuf::ChatMsgReq,msg,Msg);

};

