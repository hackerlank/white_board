
#include <sstream>

#include "Login.pb.h"
#include "WhiteBoardMessage.pb.h"
#include "EnterRoom.pb.h"
#include "RemoveShape.pb.h"

#include "SocketThread.h"
#include "OpCode.h"
#include "ResPonseThread.h"
#include "PacketHandler.h"




#pragma comment(lib, "libprotobuf.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment(lib,"pthreadVC2.lib")



/*
说明:  login  无用,仅作为测试
以下例子是使用protobuf 通信的过程
SendTest 向服务器发送数据
main　中的while循环中处理收到服务器的请求,使用时,如　PacketHandler::HandleLogin　　自行添加相应函数
*/
void SendTestLogin(SocketThread* th){

	char login_name[20] = { 0 };
	char login_passwd[20] = { 0 };
	sprintf(login_name, "name_%d", 1);
	sprintf(login_passwd, "passwd_%d", 1);
	
	Login login;
	login.set_loginname(login_name);
	login.set_loginpwd(login_passwd);
	std::string str_messgae = "";
	login.SerializeToString(&str_messgae);

	NetPacket packet(eLogin);
	packet << str_messgae;
	th->getSocket().Send(&packet);


}

/*
说明:
客户端发给服务器,lists.size() <= 1,否则视为非法刷(防止攻击)
服务器分会出现 lists.size() 可能为多个 (例如,当玩家刚进入房间,同步的时候 )

增加图元  shape_id 传0  ,服务器广播回来的 为赋值id
修改图元  shape_id 为要修改的id 
*/
void SendTestAddShape(SocketThread* th,int32 shape_id){

	WhiteBoardMessageList lists;
	WhiteBoardMessage * write_shape = lists.add_shapeobject();
	write_shape->set_shapeid(shape_id);
	write_shape->set_shapetype("aaa");
	write_shape->set_shapedata("ff");
	write_shape->set_shapeproperty("ddddd");

	/*
	真是场景中不应该有一次添加两个图元的情况,服务器将其视为非法操作
	*/
	//WhiteBoardMessage * write_shape1 = lists.add_shapeobject();
	//write_shape1->set_shapeid(2);
	//write_shape1->set_shapetype("111");
	//write_shape1->set_shapedata("22");
	//write_shape1->set_shapeproperty("33");


	std::string str_messgae = "";
	lists.SerializeToString(&str_messgae);

	NetPacket packet(eAddShape);
	packet << str_messgae;
	th->getSocket().Send(&packet);


}


/*
说明:
进桌,room_key 是桌子的唯一标识
*/
void SendTestEnterRoom(SocketThread* th){
	EnterRoom enter_req;

	enter_req.set_room_key("test");
	std::string str_messgae = "";
	enter_req.SerializeToString(&str_messgae);
	NetPacket packet(eEnterRoom);
	packet << str_messgae;
	th->getSocket().Send(&packet);
}

/*
说明:
删除图元
*/
void SendRemoveShape(SocketThread* th, int32 shape_id){
	RemoveShape remove_req;

	remove_req.set_shapeid(shape_id);

	std::string str_messgae = "";
	remove_req.SerializeToString(&str_messgae);
	NetPacket packet(eRemoveShape);
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
	while (!th->connectOk()){		//等待连接成功
		Sleep(30);
		if (ret >1000){//30秒没连上
			printf("[main] connect timeout ");
		}
	}

	SendTestEnterRoom(th);
	//SendTestAddShape(th,0);
	//SendRemoveShape(th,1);

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