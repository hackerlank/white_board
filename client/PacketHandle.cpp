
#include "WhiteBoardMessage.pb.h"
#include "RemoveShape.pb.h"
#include "ErrorResp.pb.h"
#include "RoomListResp.pb.h"
#include "PlayerListResp.pb.h"


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
	req_handler_[ProtoBuff_Add_Sharp] = &PacketHandler::HandleAddShape;
	req_handler_[ProtoBuff_Remove_Sharp] = &PacketHandler::HandleRemoveShape;
	req_handler_[ProtoBuff_Error_Info] = &PacketHandler::HandleErrorInfo;
	req_handler_[ProtoBuff_Sync_Room_List] = &PacketHandler::HandleRoomList;
	req_handler_[ProtoBuff_Sync_Player_List] = &PacketHandler::HandlePlayerList;
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


	printf("[PacketHandler::HandleErrorInfo], erro_code = %d   extra_info = %d   error_descption = %s \n",
		resp.errorcode(),resp.extrainfo(),resp.errordescription().c_str());

	return ECommonSuccess;
}

int32 PacketHandler::HandleRoomList(INetPacket& request){
	std::string tmp;
	request >> tmp;

	RoomListResp list_room;
	list_room.ParseFromString(tmp);

	std::string room_list_str = " [PacketHandler::HandleRoomList]  room_list = [";
	for (auto it = list_room.room_list().begin(); it != list_room.room_list().end();++it)
	{
		room_list_str += ",";
		room_list_str += *it;

	}
	room_list_str += "]\n";
	printf(room_list_str.c_str());
	return ECommonSuccess;
}
int32 PacketHandler::HandlePlayerList(INetPacket& request){
	std::string tmp;
	request >> tmp;
	PlayerListResp list_player;
	list_player.ParseFromString(tmp);
	std::string room_key = list_player.room_key();
	std::string player_list_str = " [PacketHandler::HandlePlayerList]  room_key = ";
	player_list_str += room_key;
	player_list_str += " [";
	for (auto it = list_player.player_list().begin(); it != list_player.player_list().end(); ++it)
	{
		player_list_str += ",";
		player_list_str += *it;
	}
	player_list_str += "]\n";
	printf(player_list_str.c_str());
	return ECommonSuccess;
}

