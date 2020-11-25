#include "AddressServer.h"

#include <string>
#include <iostream>

#include "CommandInfo.h"
#include "InputMemoryStream.h"

#ifdef _WIN32
AddressServer::AddressServer(PCWSTR ipAddress)
{
	SockAddress host(ipAddress, "address_server", 8081);
	m_socket = new UdpSocket(host);
	m_isRunning = !m_socket->inError();
}
#else

#endif

AddressServer::~AddressServer()
{
	for (int i = 0; i < m_addresses.size(); ++i)
	{
		delete m_addresses[i];
	}
	m_socket->closeSocket();
	delete m_socket;
}

void AddressServer::startServer()
{
	while (m_isRunning)
	{
		char *commandBuffer = static_cast<char *>(std::malloc(sizeof(CommandInfo)));
		SockAddress *remote = nullptr;
		int received = m_socket->receiveFrom(commandBuffer, sizeof(CommandInfo), &remote);
		InputMemoryStream stream(commandBuffer, received);
		CommandInfo cmdInfo;
		cmdInfo.read(stream);
		switch (cmdInfo.type)
		{
		case CMD_CONNECTED:
		{
			char *data = cmdInfo.data;
			data[cmdInfo.dataLength] = '\0';
			std::string identifier(data);
			remote->setIdentifier(identifier);
			m_addresses.push_back(remote);
			break;
		}
		case CMD_DISCONNETED:
		{
			char *data = cmdInfo.data;
			data[cmdInfo.dataLength] = '\0';
			std::string identifier(data);
			auto iterator = m_addresses.end();
			for (auto i = m_addresses.begin(); i != m_addresses.end(); ++i)
			{
				if ((*i)->identifier() == identifier)
				{
					iterator = i;
					break;
				}
			}
			m_addresses.erase(iterator);
			break;
		}
		case CMD_GET_ADDRESSES:
		{
			break;
		}
		}
		std::cout << "number of hosts" << std::endl;
		std::cout << m_addresses.size() << std::endl;
		if (m_addresses.size() == 0)
			stopServer();
	}
}

void AddressServer::stopServer()
{
	m_isRunning = false;
}
