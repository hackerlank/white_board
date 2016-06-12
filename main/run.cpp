
#include <sstream>

#include "SocketThread.h"
#include "OpCode.h"
#include "ResPonseThread.h"
#include "PacketHandler.h"


#include "Login.pb.h"

#pragma comment(lib, "libprotobuf.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment(lib,"pthreadVC2.lib")



/*
˵��:
����������ʹ��protobuf ͨ�ŵĹ���
SendTest ���������������
main���е�whileѭ���д����յ�������������,ʹ��ʱ,�硡PacketHandler::HandleLogin�������������Ӧ����
*/


void SendTest(SocketThread* th){

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

void main(){
	SocketThread* th = SocketThread::GetInstance();
	th->start();

	PacketHandler packet_handler;

	Sleep(100);

	SendTest(th);


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