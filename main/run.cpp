
#include "SocketThread.h"
#include "ResPonseThread.h"

void main(){
	SocketThread* th = SocketThread::GetInstance();
	th->start();
	Sleep(100);

	for (int i = 1; i < 256; i++)
	{
		/*
		buf[0] buf[1] ��ʾ���ݰ�����
		buf[2] buf[3] ��ʾop_code �¼�id
		buf[4-7]��ʾ�ð���ʵ������
		*/
		char buf[1024] = { 0 };
		buf[0] = 0;
		buf[1] = 8;
		buf[2] = 0;
		buf[3] = i;
		buf[4] = 0;
		buf[5] = 0;
		buf[6] = 0;
		buf[7] = 0;
		th->getSocket().Send(buf, 8, 0);
	}

	ResPonseThread* th_res = ResPonseThread::GetInstance();
	while (true)
	{
		INetPacket * packet=NULL;
		th_res->PopPacket(packet);
		if (packet)
		{
			printf("get packet from message queque,  opcode =  %d \n", packet->GetOpcode());
			PACKET_DELETE packet;
		}

	}
	Sleep(1000000);
}