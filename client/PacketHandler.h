#ifndef _PACKET_HANDLER_H_
#define _PACKET_HANDLER_H_

#include "BaseDef.h"

class INetPacket;


class PacketHandler{
public:

	PacketHandler();
	virtual ~PacketHandler();


public:
	void RegHandlerFuction();
	int32 HandleRequest(uint16 op_code, INetPacket &request);
protected:
private:
	int32 HandleLogin(INetPacket &request);
	int32 HandleAddShape(INetPacket &request);
	int32 HandleRemoveShape(INetPacket& request);
	int32 HandleErrorInfo(INetPacket& request);
};

#endif // _PACKET_HANDLER_H_