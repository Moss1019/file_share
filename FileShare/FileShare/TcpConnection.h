#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else

#endif

#include <string>

#include "SockAddress.h"
#include "OutputMemoryStream.h"

class TcpConnection
{
private:
	SOCKET m_sock;

	bool m_inError = false;

	bool m_isRunning = false;

	std::string m_errorMsg;

public:
	TcpConnection(const SockAddress &host, const SockAddress &remote);

	~TcpConnection();

	bool inError() const;

	const std::string &errorMsg() const;

	int sendData(OutputMemoryStream &stream);
};

