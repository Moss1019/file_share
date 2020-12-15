#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

#include <string>
#include <thread>

#include "SockAddress.h"
#include "InputMemoryStream.h"

class TcpSocket
{
private:
#ifdef _WIN32
	SOCKET m_sock;
#else
	int m_sock;
#endif

	bool m_isRunning = false;

	bool m_inError = false;

	std::thread *m_listenThread = nullptr;

	std::string m_errorMsg;

	void acceptConnection();

	void (*receiveCallback)(InputMemoryStream &stream);

public:
	TcpSocket(const SockAddress &host, void (*receiveCallback)(InputMemoryStream &stream));

	TcpSocket(const TcpSocket &other) = delete;

	TcpSocket &operator=(const TcpSocket &other) = delete;

	~TcpSocket();

	void stop();
};

