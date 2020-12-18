#include "UpdSocket.h"

UdpSocket::UdpSocket(const SockAddress &address)
	:m_inError(false), m_errorMsg("")
{
	m_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_sock < 0)
	{
		m_inError = true;
		m_errorMsg = "Could not create socket";
	}
	else
	{
		int bindRed = bind(m_sock, address.constAddress(), address.addressLen());
		if (bindRed < 0)
		{
			m_inError = true;
			m_errorMsg = "Could not bind socket";
		}
	}
}

UdpSocket::~UdpSocket()
{
    closeSocket();
}

bool UdpSocket::inError() const
{
	return m_inError;
}

const std::string &UdpSocket::errorMsg() const
{
	return m_errorMsg;
}

int UdpSocket::sendTo(const char *data, unsigned dataSize, const SockAddress &destination)
{
	if (m_inError)
	{
		return -1;
	}
	int bytesSent = sendto(m_sock, data, dataSize, 0, destination.constAddress(), destination.addressLen());
	if (bytesSent < 0)
	{
		m_inError = true;
		m_errorMsg = "Could not send to destination";
	}
	return bytesSent;
}

int UdpSocket::receiveFrom(char *data, unsigned maxSize, SockAddress **remote)
{
	if (m_inError)
	{
		return -1;
	}
	sockaddr remoteHost;
	sock_addr_size remoteHostSize = sizeof(sockaddr);
	int bytesReceived = recvfrom(m_sock, data, maxSize, 0, &remoteHost, &remoteHostSize);
	*remote = new SockAddress(remoteHost);
	if (bytesReceived < 0)
	{
		m_inError = true;
		m_errorMsg = "Could not receive data";
	}
	return bytesReceived;
}

int UdpSocket::receiveFrom(char *data, unsigned maxSize)
{
	if (m_inError)
	{
		return -1;
	}
	sockaddr remoteHost;
	sock_addr_size remoteHostSize = sizeof(sockaddr);
	int bytesReceived = recvfrom(m_sock, data, maxSize, 0, &remoteHost, &remoteHostSize);
	SockAddress sa(remoteHost);
	if (bytesReceived < 0)
	{
		m_inError = true;
		m_errorMsg = "Could not receive data";
	}
	return bytesReceived;
}

bool UdpSocket::closeSocket()
{
	if (m_inError)
	{
		return m_inError;
	}
#ifdef _WIN32
	shutdown(m_sock, SD_BOTH);
	closesocket(m_sock);
#else
	shutdown(m_sock, SHUT_RDWR);
	close(m_sock);
#endif
	m_inError = true;
	m_errorMsg = "Socket is closed";
	return m_inError;
}
