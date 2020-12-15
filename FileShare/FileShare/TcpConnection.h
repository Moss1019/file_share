#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else

#endif

#include <thread>

#include "SockAddress.h"
#include "InputMemoryStream.h"

class TcpConnection
{
private:
#ifdef _WIN32
	SOCKET m_sock;
#else
	int m_sock;
#endif

	SockAddress *m_remote;

	std::thread *m_receiveThread = nullptr;

	void (*receiveCallback)(InputMemoryStream &stream);

	void receive();

public:
#ifdef _WIN32
	TcpConnection(SOCKET sock, void (*receiveCallback)(InputMemoryStream &stream));
#else
	TcpConnection(int sock);
#endif

	TcpConnection(TcpConnection &other) = delete;

	TcpConnection &operator=(const TcpConnection &other) = delete;

	~TcpConnection();

};

