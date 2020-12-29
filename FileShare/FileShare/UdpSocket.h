#pragma once

#ifdef _WIN32

typedef int sock_addr_size;
#include <WinSock2.h>

#else

typedef unsigned sock_addr_size;
#include <unistd.h>

#endif

#include <string>

#include "SockAddress.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

class UdpSocket
{
private:
	bool m_inError;

	std::string m_errorMsg;

	int m_sock;

public:
	UdpSocket(const SockAddress &address);

	~UdpSocket();

	bool inError() const;

	const std::string &errorMsg() const;

	int sendTo(OutputMemoryStream &stream, const SockAddress &destination);

	int receiveFrom(OutputMemoryStream &stream, sockaddr *remote);

	int receiveFrom(OutputMemoryStream &stream);

	bool closeSocket();

	UdpSocket(const UdpSocket &other) = delete;

	UdpSocket &operator=(const UdpSocket &other) = delete;
};

