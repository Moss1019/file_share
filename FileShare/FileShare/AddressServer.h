#pragma once

#include <vector>

#include "OutputMemoryStream.h"
#include "SockAddress.h"
#include "UpdSocket.h"

class AddressServer
{
private:
	bool m_isRunning;

	UdpSocket *m_socket;

	std::vector<SockAddress *> m_addresses;

	void fillAddressStream(OutputMemoryStream &stream);

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

