#pragma once
#include <string.h>
#include "ConvertEndianUtil.h"
#include "ODSocket.h"
#include "SocketThread.h"
typedef struct messageHead{
	short len;
	short code;
	int   playerid;
} messagehead;
template  <typename  Rquest> class BaseRequestMsg
{
public:
	BaseRequestMsg(void);
	~BaseRequestMsg(void);
	void setRequestMessage(Rquest message);// 设置请求体
	void setMessageHead(short code,int player=0);// 设置 请求头
	bool sendMessage();// 发送信息
private:
	Rquest requestmessage;
	messagehead messageHead;
	char* getSendMessage();	
	short dateLength;
	std::string requestMessage;	
};

template  <typename  Rquest>
BaseRequestMsg<Rquest>::BaseRequestMsg(void){
}
template  <typename  Rquest>
BaseRequestMsg<Rquest>::~BaseRequestMsg(void){
}

template  <typename  Rquest>
void BaseRequestMsg<Rquest>::setRequestMessage(Rquest message){
	std::string data; 
	message.SerializeToString(&data);
	this->requestMessage=data;
}

template  <typename  Rquest>
void BaseRequestMsg<Rquest>::setMessageHead(short code,int player){
	messageHead.code=ConvertEndianUtil::convertEndianShort(code);
	messageHead.playerid=ConvertEndianUtil::convertForInt(player);
}

template  <typename  Rquest>
char* BaseRequestMsg<Rquest>::getSendMessage(){
	short total=8+requestMessage.length();
	dateLength=total;
	messageHead.len=ConvertEndianUtil::convertEndianShort(total);
	char* requestmessage=new char[total];
	char* requestmessagehead=(char*)&messageHead;	
	int i=sizeof(messageHead);
	int len=sizeof(requestMessage.c_str())/sizeof(char);
	memcpy(requestmessage,requestmessagehead,8);
	memcpy(&requestmessage[8],requestMessage.c_str(),requestMessage.length());
	return requestmessage;
}

template  <typename  Rquest>
bool BaseRequestMsg<Rquest>::sendMessage(){	
	ODSocket cSocket=SocketThread::GetInstance()->getSocket();
	char* dd=this->getSendMessage();
	int cout=cSocket.Send(this->getSendMessage(),this->dateLength,0);
	if(cout==this->dateLength){
		return true;
	}else {
		return false;
	}
}
