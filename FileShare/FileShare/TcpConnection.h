#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#ifndef socktype
#define socktype SOCKET
#endif
#else
#include <unistd.h>
#ifndef socktype
#define socktype int
#endif
#endif

#include <string>
#include <thread>

#include "SockAddress.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

class TcpConnection
{
private:
    socktype m_sock;

	bool m_inError = false;

	bool m_isRunning = false;

	std::string m_errorMsg;
    
    std::thread *m_receiveThread = nullptr;
    
    void receiveCallback();
    
    void (*onReceive)(InputMemoryStream &stream, TcpConnection *client);

public:
	TcpConnection(const SockAddress &host, const SockAddress &remote, void (*onReceive)(InputMemoryStream &stream, TcpConnection *client));
    
    TcpConnection(socktype sock, void (*onReceive)(InputMemoryStream &stream, TcpConnection *client));

	~TcpConnection();

	bool inError() const;

	const std::string &errorMsg() const;

	int sendData(OutputMemoryStream &stream);
    
    void stop();
};

