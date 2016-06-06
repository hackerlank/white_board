#include "ResPonseThread.h"
//#include "cocos2d.h"
#include "SocketThread.h"
#include "BaseResponseMsg.h"
ResPonseThread* ResPonseThread::m_pInstance=new ResPonseThread; 
ResPonseThread* ResPonseThread::GetInstance(){	
	return m_pInstance;
}
ResPonseThread::ResPonseThread(void)
{

	//this->m_msglistener=NULL;

	started = detached = false;
}


ResPonseThread::~ResPonseThread(void)
{
	stop();
}
int ResPonseThread::start(void * param){    	
	int errCode = 0;
	do{
		pthread_attr_t attributes;
		errCode = pthread_attr_init(&attributes);
		//CC_BREAK_IF(errCode!=0);
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

void* ResPonseThread::threadFunc(void *arg){
	ResPonseThread* thred=(ResPonseThread*)arg;	
	ODSocket csocket=SocketThread::GetInstance()->getSocket();
	if(SocketThread::GetInstance()->state==0){
		while(true){
			// 表示服务器端 有消息推送过来
			if(csocket.Select()==-2){				
				char recvBuf[8];// 获取请求头的 数据
				int i=	csocket.Recv(recvBuf,8,0);				
				if (i==8){				
					char dc1[2]={recvBuf[1],recvBuf[0]};
					short len = *(short*)&dc1[0];
					char dc2[2]={recvBuf[3],recvBuf[2]};
					short code = *(short*)&dc2[0];
					char dc3[4]={recvBuf[7],recvBuf[6],recvBuf[5],recvBuf[4]};
					int playId=*(int*)&dc3[0];
					//CCLOG("%d",playId);
					char* messbody=NULL;
					int myl=0;
					if(len>8){							
						myl=len-8;
						messbody=new char[myl];						
						csocket.Recv(messbody,myl,0);					  
					}
					//	//1001 = com.lx.command.player.LoginCmd
					//1002 = com.lx.command.player.RegisterCmd
					//1003 = com.lx.command.player.HeartBeatCmd
					// 登陆

					BaseResponseMsg* basmsg=new BaseResponseMsg();
					basmsg->code=code;
					basmsg->len=len;
					basmsg->playerId=playId;
					// 表示服务器推动过来的消息
					if(code==1000){
						//if(thred->m_msglistener){
						//basmsg->setStringToMsg(messbody,myl);
						//(thred->m_msglistener->*(thred->msgselector))(basmsg);
						//}
					}
					else {
						//CCLOG("%d",code);
					}
				}else {
					//if(thred->m_notconlistener){
					// BaseResponseMsg* basmsg=new BaseResponseMsg();
					// basmsg->state=1;// 连接断开
					//(thred->m_notconlistener->*(thred->notconselector))(basmsg);
					//}
					break;
				}

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