

#include "Login.pb.h"
#include "WhiteBoardMessage.pb.h"
#include "RemoveShape.pb.h"
#include "ErrorResp.pb.h"

#include "PacketHandler.h"
#include "BaseDef.h"
#include "NetPacket.h"
#include "OpCode.h"
#include "SocketThread.h"



typedef int32(PacketHandler::*ReqHandler)(INetPacket&);
static std::map<uint32, ReqHandler > req_handler_;



PacketHandler::PacketHandler(){

	RegHandlerFuction();
}

PacketHandler::~PacketHandler(){

}


void PacketHandler::RegHandlerFuction(){
	req_handler_[eLogin] = &PacketHandler::HandleLogin;
	req_handler_[eAddShape] = &PacketHandler::HandleAddShape;
	req_handler_[eRemoveShape] = &PacketHandler::HandleRemoveShape;
	req_handler_[eErrorCode] = &PacketHandler::HandleErrorInfo;

}

int32 PacketHandler::HandleRequest(uint16 op_code, INetPacket &request){

	auto it = req_handler_.find(op_code);
	if (it != req_handler_.end()) {
		return (this->*(it->second))(request);
	}
	else
	{
		printf ("[PacketHandler::HandleRequest]   ERROR ,   opcode = %d " , op_code);
		return EErrorNoHandlerFun;
	}

	return ECommonSuccess;
}


/*
以下函数演示使用方法
客户端每次收到 login包时,将name 自增再发给服务器
*/
int32 PacketHandler::HandleLogin(INetPacket &request){
	std::string tmp;
	request >> tmp;
	Login get_packet;
	get_packet.ParseFromString(tmp);
	//结果输出
	printf("[PacketHandler::HandleLogin], login_name = %s    paswdd = %s  \n",
		get_packet.loginname().c_str(), get_packet.loginpwd().c_str());

	//解析一下
	std::string name = get_packet.loginname();
	name = name.substr(name.find_last_of("_")+1, name.length() - name.find_last_of("_"));
	int i =atoi(name.c_str());


	if (i >= 100)
	{
		return ECommonSuccess;
	}

	//将数字自增 再发给服务器
	i++;
	char login_name[20] = { 0 };
	char login_passwd[20] = { 0 };
	sprintf(login_name, "name_%d", i);
	sprintf(login_passwd, "passwd_%d", i);

	Login login;
	login.set_loginname(login_name);
	login.set_loginpwd(login_passwd);

	std::string str_messgae = "";
	login.SerializeToString(&str_messgae);

	NetPacket packet(eLogin);
	packet << str_messgae;
	SocketThread::GetInstance()->getSocket().Send(&packet);

	return ECommonSuccess;
}



int32 PacketHandler::HandleAddShape(INetPacket &request){

	std::string tmp;
	request >> tmp;
	WhiteBoardMessageList resp;
	resp.ParseFromString(tmp);

	for (int i = 0; i< resp.shapeobject_size(); i++)
	{
		WhiteBoardMessage  mess = resp.shapeobject(i);
		printf("[PacketHandler::HandleAddShape], shape_id = %d    shape_type = %s    shape_data =%s   shape_property = %s \n",
			mess.shapeid(), mess.shapetype().c_str(), mess.shapedata().c_str(), mess.shapeproperty().c_str());
	}



	return ECommonSuccess;

}


/*
删除图元
*/
int32 PacketHandler::HandleRemoveShape(INetPacket& request){

	std::string tmp;
	request >> tmp;

	RemoveShape resp;
	resp.ParseFromString(tmp);

	printf("[PacketHandler::HandleRemoveShape], shape_id = %d   \n",
		resp.shapeid());

	return ECommonSuccess;
}


/*
错误处理函数
*/
int32 PacketHandler::HandleErrorInfo(INetPacket& request){

	std::string tmp;
	request >> tmp;

	ErrorResp resp;
	resp.ParseFromString(tmp);


	printf("[PacketHandler::HandleRemoveShape], erro_code = %d   extra_info = %d   error_descption = %s \n",
		resp.errorcode(),resp.extrainfo(),resp.errordescription().c_str());

	return ECommonSuccess;
}