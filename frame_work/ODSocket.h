/*
 * define file about portable socket class.
 * description:this sock is suit both windows and linux
 * design:odison
 * e-mail:odison@126.com>
 *
 */

#ifndef _ODSOCKET_H_
#define _ODSOCKET_H_


#include <winsock2.h>
typedef int				socklen_t;



class ODSocket {

public:
	ODSocket(SOCKET sock = INVALID_SOCKET);
	~ODSocket();

	// Create socket object for snd/recv data
	bool Create(int af, int type, int protocol = 0);

	// Connect socket
	bool Connect(const char* ip, unsigned short port);
	//#region server
	// Bind socket
	bool Bind(unsigned short port);

	// Listen socket
	bool Listen(int backlog = 5);

	// Accept socket
	bool Accept(ODSocket& s, char* fromip = NULL);
	//#endregion
	int Select();
	// Send socket
	int Send(const char* buf, int len, int flags = 0);

	// Recv socket
	int Recv(char* buf, int len, int flags = 0);

	// Close socket
	int Close();

	// Get errno
	int GetError();

	//#pragma region just for win32
	// Init winsock DLL
	static int Init();
	// Clean winsock DLL
	static int Clean();
	//#pragma endregion

	// Domain parse
	static bool DnsParse(const char* domain, char* ip);

	ODSocket& operator = (SOCKET s);

	operator SOCKET ();

protected:
	SOCKET m_sock;
	fd_set  fdR;
};

#endif
