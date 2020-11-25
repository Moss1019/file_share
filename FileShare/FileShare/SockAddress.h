#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

#include <string>
#include <memory>

class SockAddress
{
private:
	sockaddr m_address;

	sockaddr_in *addressAsSockAddrIn();

	std::string m_identifier;

public:
#ifdef _WIN32
	SockAddress(const PCWSTR ipAddress, const std::string &identifier, unsigned short port);
#else 
	SockAddress(const std::string &ipAddress, const std::string &identifier, unsigned short port);
#endif

	SockAddress(const sockaddr &address);

	const sockaddr *address() const;

	unsigned addressLen() const;

	const std::string &identifier() const;

	void setIdentifier(const std::string &identifier);

	std::string getInetAddress();

	SockAddress(const SockAddress &other) = delete;

	SockAddress &operator=(const SockAddress &other) = delete;
};

typedef std::shared_ptr<SockAddress> SockAddressPtr;

