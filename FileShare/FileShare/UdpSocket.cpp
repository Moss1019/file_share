#include "UdpSocket.h"

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

int UdpSocket::sendTo(OutputMemoryStream &stream, const SockAddress &destination)
{
	if (m_inError)
	{
		return -1;
	}
	int bytesSent = sendto(m_sock, stream.getBufferPtr(), stream.getLength(), 0, destination.constAddress(), destination.addressLen());
	if (bytesSent < 0)
	{
		m_inError = true;
		m_errorMsg = "Could not send to destination";
	}
	return bytesSent;
}

int UdpSocket::receiveFrom(OutputMemoryStream &stream, sockaddr *remote)
{
	if (m_inError)
	{
		return -1;
	}
	sock_addr_size remoteHostSize = sizeof(sockaddr);
	char *buffer = reinterpret_cast<char *>(std::malloc(512));
	int bytesReceived = recvfrom(m_sock, buffer, 512, 0, remote, &remoteHostSize);
	if (bytesReceived < 0)
	{
		m_inError = true;
		m_errorMsg = "Could not receive data";
	}
	stream.write(buffer, bytesReceived);
	std::free(buffer);
	return bytesReceived;
}

int UdpSocket::receiveFrom(OutputMemoryStream &stream)
{
	if (m_inError)
	{
		return -1;
	}
	sockaddr remoteHost;
	sock_addr_size remoteHostSize = sizeof(sockaddr);
	char *buffer = reinterpret_cast<char *>(std::malloc(512));
	int bytesReceived = recvfrom(m_sock, buffer, 512, 0, &remoteHost, &remoteHostSize);
	if (bytesReceived < 0)
	{
		m_inError = true;
		m_errorMsg = "Could not receive data";
	}
	std::free(buffer);
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
