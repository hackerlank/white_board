#include "BaseDef.h"
#include "NetPacket.h"

volatile long INetPacket::s_total_cnt = 0;

void INetPacket::WritePacket(const INetPacket& r)
{
	uint32 opcode = r.GetOpcode();

	NetPacketHeader Header;
	Header.size = _BITSWAP32((uint32)( r.Size() + PACKET_HEADER_SIZE ) );
	Header.cmd = _BITSWAP32(opcode);

	_Write((uint8*)&Header, PACKET_HEADER_SIZE);
	size_t size = r.GetRemainSize();
	Append(r.GetReadBuffer(), size);
};

INetPacket * INetPacket::ReadPacket(INetPacket* r)
{
	NetPacketHeader Header;
	_Read( (uint8*)&Header,PACKET_HEADER_SIZE);
	Header.size = _BITSWAP32(Header.size );
	Header.cmd = _BITSWAP32(Header.cmd);
	uint32 size = Header.size - PACKET_HEADER_SIZE;
	if (!r)
	{
		r = NEW_NETPACKET(size, Header.cmd);
	}
	else
	{
		r->SetOpcode(Header.cmd);
	}
	_Read(r->GetWriteBuffer(size), size);
	return r;
};

INetPacket * INetPacket::UnPackPacket()
{
	NetPacketHeader Header;
	_Read( (uint8*)&Header,PACKET_HEADER_SIZE);
	//Header.size = BITSWAP16(Header.size );
	Header.cmd  = _BITSWAP16(Header.cmd  );
	//uint16 size = Header.size - PACKET_HEADER_SIZE;
	SetOpcode(Header.cmd);
	//_Read(r->GetWriteBuffer(size), size);
	return this;
};

INetPacket * INetPacket::CopyNew() const
{
	INetPacket* r = NEW_NETPACKET((uint32)Size(), _opcode);
	r->_Write(Content(), Size());
	return r;
};

void InitNetPackPools()
{
#if (MAX_POOL_MEM > 0)
	CNetPacket<32>::InitPool();
	CNetPacket<64>::InitPool();
	CNetPacket<128>::InitPool();
	CNetPacket<256>::InitPool();
	CNetPacket<512>::InitPool();
	CNetPacket<1024>::InitPool();
	CNetPacket<4096>::InitPool();
	CNetPacket<8192>::InitPool();
	CNetPacket<32768>::InitPool();
	CNetPacket<65536>::InitPool();
#endif
}

void ClearNetPackPools()
{
#if (MAX_POOL_MEM > 0)
	CNetPacket<32>::ClearPool();
	CNetPacket<64>::ClearPool();
	CNetPacket<128>::ClearPool();
	CNetPacket<256>::ClearPool();
	CNetPacket<512>::ClearPool();
	CNetPacket<1024>::ClearPool();
	CNetPacket<4096>::ClearPool();
	CNetPacket<8192>::ClearPool();
	CNetPacket<32768>::ClearPool();
	CNetPacket<65536>::ClearPool();
#endif
}



