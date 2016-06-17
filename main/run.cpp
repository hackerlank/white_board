
#include <sstream>


#include "WhiteBoardMessage.pb.h"
#include "RoomOperate.pb.h"
#include "RemoveShape.pb.h"
#include "RoomOperate.pb.h"

#include "SocketThread.h"
#include "OpCode.h"
#include "ResPonseThread.h"
#include "PacketHandler.h"




#pragma comment(lib, "libprotobuf.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment(lib,"pthreadVC2.lib")







/*
���󷿼��б�
*/
void TestAskRoomList(SocketThread* th){
	RoomOperate room_operate;
	room_operate.set_oprate_type(RoomOperate_Ask_Room_List);
	room_operate.set_use_name(USR_NAME);

	std::string str_messgae = "";
	room_operate.SerializeToString(&str_messgae);
	NetPacket packet(ProtoBuff_Room_Operate);
	packet << str_messgae;
	th->getSocket().Send(&packet);
}


/*
��������
*/
void TestCreateRoom(SocketThread* th,std::string& room_name){
	RoomOperate room_operate;
	room_operate.set_oprate_type(RoomOperate_Create_Room);
	room_operate.set_use_name(USR_NAME);
	room_operate.set_room_key_origin(room_name);

	std::string str_messgae = "";
	room_operate.SerializeToString(&str_messgae);
	NetPacket packet(ProtoBuff_Room_Operate);
	packet << str_messgae;
	th->getSocket().Send(&packet);
}

/*
���뷿��
*/
void TestEnterRoom(SocketThread* th, std::string& room_name){
	RoomOperate room_operate;
	room_operate.set_oprate_type(RoomOperate_Enter_Room);
	room_operate.set_use_name(USR_NAME);
	room_operate.set_room_key_origin(room_name);

	std::string str_messgae = "";
	room_operate.SerializeToString(&str_messgae);
	NetPacket packet(ProtoBuff_Room_Operate);
	packet << str_messgae;
	th->getSocket().Send(&packet);
}

/*
�뿪����
*/
void TestLeaveRoom(SocketThread* th ,std::string& room_name){
	RoomOperate room_operate;
	room_operate.set_oprate_type(RoomOperate_Leave_Room);
	room_operate.set_use_name(room_name);
	room_operate.set_room_key_origin(ROOM_NAME);

	std::string str_messgae = "";
	room_operate.SerializeToString(&str_messgae);
	NetPacket packet(ProtoBuff_Room_Operate);
	packet << str_messgae;
	th->getSocket().Send(&packet);
}

/*
ɾ������
*/
void TestDeleteRoom(SocketThread* th, std::string& room_name){
	RoomOperate room_operate;
	room_operate.set_oprate_type(RoomOperate_Delete_Room);
	room_operate.set_use_name(USR_NAME);
	room_operate.set_room_key_origin(room_name);

	std::string str_messgae = "";
	room_operate.SerializeToString(&str_messgae);
	NetPacket packet(ProtoBuff_Room_Operate);
	packet << str_messgae;
	th->getSocket().Send(&packet);
}

/*
��������
*/
void TestCopyRoom(SocketThread* th){
	RoomOperate room_operate;
	room_operate.set_oprate_type(RoomOperate_Copy_Room);
	room_operate.set_use_name(USR_NAME);
	room_operate.set_room_key_origin(ROOM_NAME);
	room_operate.set_room_key_destination(ROOM_COPY_TO);

	std::string str_messgae = "";
	room_operate.SerializeToString(&str_messgae);
	NetPacket packet(ProtoBuff_Room_Operate);
	packet << str_messgae;
	th->getSocket().Send(&packet);
}

/*
���󷿼��б�
*/
void TestAskPlayerList(SocketThread* th, std::string& room_name){
	RoomOperate room_operate;
	room_operate.set_oprate_type(RoomOperate_Ask_Player_List);
	room_operate.set_use_name(USR_NAME);
	room_operate.set_room_key_origin(room_name);

	std::string str_messgae = "";
	room_operate.SerializeToString(&str_messgae);
	NetPacket packet(ProtoBuff_Room_Operate);
	packet << str_messgae;
	th->getSocket().Send(&packet);
}


/*
˵��:
ɾ��ͼԪ
*/
void SendRemoveShape(SocketThread* th, int32 shape_id){
	RemoveShape remove_req;

	remove_req.set_shapeid(shape_id);

	std::string str_messgae = "";
	remove_req.SerializeToString(&str_messgae);
	NetPacket packet(ProtoBuff_Remove_Sharp);
	packet << str_messgae;
	th->getSocket().Send(&packet);
}

/*
˵��:
�ͻ��˷���������,lists.size() <= 1,������Ϊ�Ƿ�ˢ(��ֹ����)
�������ֻ���� lists.size() ����Ϊ��� (����,����Ҹս��뷿��,ͬ����ʱ�� )

����ͼԪ  shape_id ��0  ,�������㲥������ Ϊ��ֵid
�޸�ͼԪ  shape_id ΪҪ�޸ĵ�id
*/
void SendTestAddShape(SocketThread* th, int32 shape_id){

	WhiteBoardMessageList lists;
	WhiteBoardMessage * write_shape = lists.add_shapeobject();
	write_shape->set_shapeid(shape_id);
	write_shape->set_shapetype("aaa");
	write_shape->set_shapedata("ff");
	write_shape->set_shapeproperty("ddddd");

	std::string str_messgae = "";
	lists.SerializeToString(&str_messgae);

	NetPacket packet(ProtoBuff_Add_Sharp);
	packet << str_messgae;
	th->getSocket().Send(&packet);

}


void main(){
	SocketThread* th = SocketThread::GetInstance();
	int ret = th->start();
	if (ret != 0){
		printf("[main] SocketThread start  error, error_no = %d", ret);
		return;
	}
	ret = 0;
	while (!th->connectOk()){		//�ȴ����ӳɹ�
		Sleep(30);
		if (ret >1000){//30��û����
			printf("[main] connect timeout ");
		}
	}

	TestCreateRoom(th, std::string(ROOM_NAME));
	TestCreateRoom(th, std::string(ROOM_COPY_TO));
	TestAskRoomList(th);
	TestEnterRoom(th, std::string(ROOM_COPY_TO));
	SendTestAddShape(th,0);
	//TestEnterRoom(th, std::string(ROOM_COPY_TO));

	//TestDeleteRoom(th, std::string(ROOM_COPY_TO));
	//TestAskPlayerList(th, std::string(ROOM_COPY_TO));

	//Sleep(1000);
	//TestAskRoomList(th);
	//TestEnterRoom(th, std::string(ROOM_NAME));
	//TestAskPlayerList(th, std::string(ROOM_NAME));

	PacketHandler packet_handler;
	ResPonseThread* th_res = ResPonseThread::GetInstance();
	while (true)
	{
		INetPacket * packet=NULL;
		th_res->PopPacket(packet);
		if (packet)
		{
			packet_handler.HandleRequest(packet->GetOpcode(), *packet);
			PACKET_DELETE packet;
		}
	}
}