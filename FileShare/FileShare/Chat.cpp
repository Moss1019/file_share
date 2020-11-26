#include "Chat.h"

#include <memory>
#include <cstring>

#include "Message.h"

void Chat::receiveHandler()
{
	while (m_isRunning)
	{
		char *buffer = static_cast<char *>(std::malloc(1024));
		try 
		{
			int received = m_socket->receiveFrom(buffer, 1024);
			InputMemoryStream stream(buffer, received);
			m_receivedCallback(stream);
		}
		catch (std::exception &err)
		{
			std::free(buffer);
		}
	}
}

Chat::Chat(const std::string &identifier, void (*receiveCallback)(InputMemoryStream &stream))
{
	m_address = new SockAddress(L"127.0.0.1", identifier, 8081);
	m_socket = new UdpSocket(*m_address);
	m_receivedCallback = receiveCallback;
	m_isRunning = !m_socket->inError();
}

Chat::~Chat()
{
	delete m_socket;
	delete m_address;
}

void Chat::sendMsg(const std::string &msg, SockAddress &remote)
{
	Message msgObj;
	msgObj.identifierLength = m_address->identifier().length();
	msgObj.messageLength = msg.length();
	msgObj.identifier = m_address->identifier();
	msgObj.message = msg;
	OutputMemoryStream stream;
	msgObj.write(stream);
	int sent = m_socket->sendTo(stream.getBufferPtr(), stream.getLength(), remote);
}
