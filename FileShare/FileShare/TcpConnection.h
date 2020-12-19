#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <unistd.h>
#endif

#include <string>
#include <thread>

#include "SockAddress.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"

class TcpConnection
{
private:
#ifdef _WIN32
	SOCKET m_sock;
#else
    int m_sock;
#endif

	bool m_inError = false;

	bool m_isRunning = false;

	std::string m_errorMsg;
    
    std::thread *m_receiveThread = nullptr;
    
    void receiveCallback();
    
    void (*onReceive)(InputMemoryStream &stream);

public:
	TcpConnection(const SockAddress &host, const SockAddress &remote, void (*onReceive)(InputMemoryStream &stream));
    
#ifdef _WIN32
    TcpConnection(SOCKET sock, void (*onReceive)(InputMemoryStream &stream));
#else
    TcpConnection(int sock, void (*onReceive)(InputMemoryStream &stream));
#endif

	~TcpConnection();

	bool inError() const;

	const std::string &errorMsg() const;
<<<<<<< HEAD

	int sendData(OutputMemoryStream &stream);
=======
    
    void stop();
>>>>>>> 6983b19d79d1ae853a4b3c92c82e6885b62e75ec
};

