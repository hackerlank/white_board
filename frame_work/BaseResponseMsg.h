#pragma once
//#include "chat_msg_req.pb.h"
/****
 �����Ҫ�ǰ� �������еõ���ֵ  ת���� ����Ҫ�Ķ���
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
	short code;// ��Ӧ���
	int playerId; // �û����
	int state;// ����� ��ǰ��״̬  0 ��ʾ ���ӳɹ� 1 ��ʾ���ӶϿ�
	short len;// ���ݳ���
	//M_SYNTHESIZE(zzboy::protobuf::ChatMsgReq,msg,Msg);

};

