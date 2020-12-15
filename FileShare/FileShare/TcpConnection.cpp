#include "TcpConnection.h"

#include "OutputMemoryStream.h"

#define BUFFERSIZE 512

void TcpConnection::receive()
{
	int bytesReceived = 0;
	int totalReceived = 0;
	char *buffer = reinterpret_cast<char *>(std::malloc(BUFFERSIZE));
	OutputMemoryStream stream;
	while ((bytesReceived = recv(m_sock, buffer, BUFFERSIZE, 0)) < 0)
	{
		stream.write(buffer, bytesReceived);
		totalReceived += bytesReceived;
	}
	InputMemoryStream inputStream(stream.getBufferPtr(), stream.getLength());
	receiveCallback(inputStream);
}

TcpConnection::TcpConnection(SOCKET sock, SockAddress *remote, void (*receiveCallback)(InputMemoryStream &stream))
	:m_sock(sock), m_remote(remote), receiveCallback(receiveCallback)
{
	m_receiveThread = new std::thread(&TcpConnection::receive, this);
}

TcpConnection::~TcpConnection()
{
	if (m_receiveThread != nullptr)
	{
		m_receiveThread->join();
		delete m_receiveThread;
		m_receiveThread = nullptr;
	}
#ifdef _WIN32
	closesocket(m_sock);
#else
	close(m_sock);
#endif
}
