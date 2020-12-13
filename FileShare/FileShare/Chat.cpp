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
#ifdef _WIN32
	m_address = new SockAddress(L"192.0168.1.100", identifier, 8081);
#else
    m_address = new SockAddress("192.168.1.176", identifier, 8081);
#endif
	m_socket = new UdpSocket(*m_address);
	m_receivedCallback = receiveCallback;
	m_isRunning = !m_socket->inError();
}

Chat::~Chat()
{
	delete m_socket;
	delete m_address;
    clearPeers();
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

void Chat::clearPeers()
{
    for(unsigned i = 0; i < m_peers.size(); ++i)
    {
        delete m_peers[i];
    }
    m_peers.clear();
}
