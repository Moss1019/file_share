#include "TcpSocket.h"

#ifdef _WIN32
#include <WinSock2.h>
#else

#endif

#include <string>

#include "TcpConnection.h"

void TcpSocket::acceptConnection()
{
#ifdef _WIN32
	SOCKET newClientSock;
#else
	int newClientSock;
#endif
	sockaddr newClient;
	memset(&newClient, 0, sizeof(sockaddr));
	int clientAddrLen = sizeof(sockaddr);
	while (m_isRunning)
	{
		newClientSock = accept(m_sock, &newClient, &clientAddrLen);
		if (newClientSock > 0)
		{
			TcpConnection con(newClientSock, receiveCallback);
		}
	}
}

TcpSocket::TcpSocket(const SockAddress &host, void (*receiveCallback)(InputMemoryStream &stream))
	:receiveCallback(receiveCallback)
{
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock < 0)
	{
		m_errorMsg = "Failed to create tcp socket";
		m_inError = true;
	}
	if (!m_inError)
	{
		if (bind(m_sock, host.address(), host.addressLen()) < 0)
		{
			m_errorMsg = "Bind failed";
			m_inError = true;
		}
	}
	if (!m_inError)
	{
		if (listen(m_sock, 5) < 0)
		{
			m_errorMsg = "Failed to start listening";
			m_inError = true;
		}
	}
	m_isRunning = !m_inError;
}

TcpSocket::~TcpSocket()
{
	stop();
}

void TcpSocket::stop()
{
	m_isRunning = false;
	if (m_listenThread != nullptr)
	{
		delete m_listenThread;
		m_listenThread = nullptr;
	}
}
