#pragma once

#include <vector>

#include "SockAddress.h"
#include "UpdSocket.h"

class AddressServer
{
private:
	bool m_isRunning;

	UdpSocket *m_socket;

	std::vector<SockAddress *> m_addresses;

public:
#ifdef _WIN32
	AddressServer(PCWSTR ipAddress);
#else
    
#endif

	~AddressServer();

	void startServer();

	void stopServer();

	AddressServer(const AddressServer &other) = delete;

	AddressServer &operator=(const AddressServer &other) = delete;


};

