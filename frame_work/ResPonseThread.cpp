#include "ResPonseThread.h"
#include "SocketThread.h"




ResPonseThread* ResPonseThread::m_pInstance = new ResPonseThread;
ResPonseThread* ResPonseThread::GetInstance(){
	return m_pInstance;
}
ResPonseThread::ResPonseThread(void)
{

	started = detached = false;
	Reset();
}

void ResPonseThread::Reset(){
	readBuffer.Allocate(CLIENTSOCKET_RECVBUF_SIZE);
	m_nRemaining = m_Opcode = 0;
}

ResPonseThread::~ResPonseThread(void)
{
	readBuffer.Reset();
	stop();
}
int ResPonseThread::start(void * param){    	
	int errCode = 0;
	do{
		pthread_attr_t attributes;
		errCode = pthread_attr_init(&attributes);
		//但是上面这个函数其他内容则主要为你创建的线程设定为分离式
		errCode = pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
		if (errCode!=0) {
			pthread_attr_destroy(&attributes);
			break;
		}		
		errCode = pthread_create(&handle, &attributes,threadFunc,this);
		started = true; 
	}while (0);
	return errCode;
} 


bool ResPonseThread::PopPacket(INetPacket*& packet){
	if (m_msgQueue.Dequeue(packet, 1000) == 0){
		return true;
	}
	return false;
}

void ResPonseThread::OnRead(){

	while (true)
	{
		if (m_nRemaining == 0)
		{
			if (GetReadBuffer().GetSize() < PACKET_HEADER_SIZE)
			{
				return;
			}

			NetPacketHeader Header;
			GetReadBuffer().Read(reinterpret_cast<uint8*>(&Header), PACKET_HEADER_SIZE);

			Header.size = _BITSWAP16(Header.size);
			Header.cmd = _BITSWAP16(Header.cmd);

			assert(Header.size >= PACKET_HEADER_SIZE);
			m_nRemaining = Header.size - PACKET_HEADER_SIZE;
			m_Opcode = Header.cmd;


		}

		if (m_nRemaining > 0)
		{
			if (GetReadBuffer().GetSize() < m_nRemaining)
			{
				return;
			}
		}

		CircularBuffer *currentBuffer = &readBuffer;
		//printf("read code = %d\n", m_Opcode);
		NetPacket *Packet = PACKET_NEW NetPacket(static_cast<uint16>(m_Opcode));
		if (m_nRemaining > 0)
		{
			currentBuffer->Read(reinterpret_cast<uint8*>(Packet->GetWriteBuffer(m_nRemaining)), m_nRemaining);
		}

		m_nRemaining = m_Opcode = 0;
		m_msgQueue.Enqueue(Packet);
	}
}

void* ResPonseThread::threadFunc(void *arg){
	ResPonseThread* thred=(ResPonseThread*)arg;	
	ODSocket csocket=SocketThread::GetInstance()->getSocket();
	if(SocketThread::GetInstance()->state==0){
		while(true){
			// 表示服务器端 有消息推送过来
			if(csocket.Select()==-2){			

				uint32 nLen = (uint32)thred->GetReadBuffer().GetSpace();
				char* pBuff = (char*)thred->GetReadBuffer().GetBuffer();

				int nRecv = csocket.Recv(pBuff, nLen, 0);
				if (nRecv == SOCKET_ERROR)
				{
					//uint32 err = SocketOps::GetErrorNumber();
					//if (err != ERROR_WOULDBLOCK)
					//{
					//	OnDisconnect(err);
					//	return false;
					//}
				}
				else if (nRecv == 0)
				{
					////DLOG("shutdown...");
					//OnDisconnect(0);
					//return false;
				}


				thred->GetReadBuffer().IncrementWritten(nRecv);
				thred->OnRead();

			}
		}
	}
	return NULL;
}


void ResPonseThread::stop(){
	if (started && !detached) { 
		pthread_cancel(handle);
		pthread_detach(handle); 
		detached = true; 
	}
}

void * ResPonseThread::wait(){
	void * status = NULL;
	if (started && !detached) { 
		pthread_join(handle, &status); 
	}
	return status;
}
void ResPonseThread::sleep(int secstr){
	timeval timeout = { secstr/1000, secstr%1000}; 
	select(0, NULL, NULL, NULL, &timeout);
}

void ResPonseThread::detach(){
	if (started && !detached) {
		pthread_detach(handle);
	} 
	detached = true;
}