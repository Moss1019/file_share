#include "TcpConnection.h"

TcpConnection::TcpConnection(const SockAddress &host, const SockAddress &remote)
{
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock < 0)
	{
		m_inError = true;
		m_errorMsg = "Socket creation failed";
	}
	if (bind(m_sock, host.constAddress(), host.addressLen()) < 0)
	{
		m_inError = true;
		m_errorMsg = "Socket binding failed";
	}
	if (connect(m_sock, remote.constAddress(), remote.addressLen()) < 0)
	{
		m_inError = true;
		m_errorMsg = "Socket connection failed";
	}
	m_isRunning = !m_inError;	
}

TcpConnection::~TcpConnection()
{
	closesocket(m_sock);
}

bool TcpConnection::inError() const
{
	return m_inError;
}

const std::string &TcpConnection::errorMsg() const
{
	return m_errorMsg;
}

int TcpConnection::sendData(OutputMemoryStream &stream)
{
	return send(m_sock, stream.getBufferPtr(), stream.getLength(), 0);
}
