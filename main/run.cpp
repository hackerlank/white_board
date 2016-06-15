
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
˵��:  login  ����,����Ϊ����
����������ʹ��protobuf ͨ�ŵĹ���
SendTest ���������������
main���е�whileѭ���д����յ�������������,ʹ��ʱ,�硡PacketHandler::HandleLogin�������������Ӧ����
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
˵��:
�ͻ��˷���������,lists.size() <= 1,������Ϊ�Ƿ�ˢ(��ֹ����)
�������ֻ���� lists.size() ����Ϊ��� (����,����Ҹս��뷿��,ͬ����ʱ�� )

����ͼԪ  shape_id ��0  ,�������㲥������ Ϊ��ֵid
�޸�ͼԪ  shape_id ΪҪ�޸ĵ�id 
*/
void SendTestAddShape(SocketThread* th,int32 shape_id){

	WhiteBoardMessageList lists;
	WhiteBoardMessage * write_shape = lists.add_shapeobject();
	write_shape->set_shapeid(shape_id);
	write_shape->set_shapetype("aaa");
	write_shape->set_shapedata("ff");
	write_shape->set_shapeproperty("ddddd");

	/*
	���ǳ����в�Ӧ����һ���������ͼԪ�����,������������Ϊ�Ƿ�����
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
˵��:
����,room_key �����ӵ�Ψһ��ʶ
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
˵��:
ɾ��ͼԪ
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
	while (!th->connectOk()){		//�ȴ����ӳɹ�
		Sleep(30);
		if (ret >1000){//30��û����
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