#pragma once

#ifdef _WIN32

typedef int sock_addr_size;
#include <WinSock2.h>

#else

typedef unsigned sock_addr_size;

#endif

#include <string>

#include "SockAddress.h"

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

	int sendTo(const char *data, unsigned dataSize, const SockAddress &destination);

	int receiveFrom(char *data, unsigned maxSize);

	bool close();

	UdpSocket(const UdpSocket &other) = delete;

	UdpSocket &operator=(const UdpSocket &other) = delete;
};

