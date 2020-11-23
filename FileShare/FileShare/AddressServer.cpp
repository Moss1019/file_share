#include "AddressServer.h"

#include <string>
#include <iostream>

#include "CommandInfo.h"

AddressServer::AddressServer(PCWSTR ipAddress)
{
	SockAddress host(ipAddress, "address_server", 8081);
	m_socket = new UdpSocket(host);
	m_isRunning = !m_socket->inError();
}

AddressServer::~AddressServer()
{
	m_socket->closeSocket();
	delete m_socket;
}

void AddressServer::startServer()
{
	char commandBuffer[sizeof(CommandInfo)];
	char dataBuffer[1024];
	while (m_isRunning)
	{
		SockAddress *remote = nullptr;
		int received = m_socket->receiveFrom(commandBuffer, sizeof(CommandInfo), remote);
		CommandInfo info;
		memset(&info, 0, sizeof(CommandInfo));
		memcpy(&info, reinterpret_cast<const void *>(commandBuffer), sizeof(CommandInfo));
		switch (info.type)
		{
		case CMD_CONNECTED:
		{
			std::string identifier(dataBuffer);
			remote->setIdentifier(identifier);
			m_addresses.push_back(remote);
			break;
		}
		case CMD_DISCONNETED:
		{

			break;
		}
		case CMD_GET_ADDRESSES:
		{
			break;
		}
		}
		std::cout << "number of hosts" << std::endl;
		std::cout << m_addresses.size() << std::endl;
	}
}

void AddressServer::stopServer()
{
	m_isRunning = false;
}
