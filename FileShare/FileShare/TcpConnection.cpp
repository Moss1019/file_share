#include "TcpConnection.h"

#include <iostream>

#include "OutputMemoryStream.h"

void TcpConnection::receiveCallback()
{
    while(m_isRunning)
    {
        int bytesReceived = 0;
        int totalReceived = 0;
        OutputMemoryStream outStream;
        char *buffer = reinterpret_cast<char *>(std::malloc(512));
        bytesReceived = recv(m_sock, buffer, 512, 0);
        std::cout << "Received data " << bytesReceived << std::endl;
        if (bytesReceived > 0)
        {
            outStream.write(buffer, bytesReceived);
            totalReceived += bytesReceived;
        }
        InputMemoryStream inStream(outStream.getBufferPtr(), outStream.getLength());
        onReceive(inStream, this);
    }
}

TcpConnection::TcpConnection(const SockAddress &host, const SockAddress &remote, void (*onReceive)(InputMemoryStream &stream, TcpConnection *client))
:onReceive(onReceive)
{
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock < 0)
	{
		m_inError = true;
		m_errorMsg = "Socket creation failed";
	}
	if (connect(m_sock, remote.constAddress(), remote.addressLen()) < 0)
	{
		m_inError = true;
		m_errorMsg = "Socket connection failed";
	}
	m_isRunning = !m_inError;	
}

TcpConnection::TcpConnection(socktype sock, void (*onReceive)(InputMemoryStream &stream, TcpConnection *client))
    :m_sock(sock), onReceive(onReceive)
{
    m_isRunning = true;
    m_receiveThread = new std::thread(&TcpConnection::receiveCallback, this);
}

TcpConnection::~TcpConnection()
{
    m_isRunning = false;
#ifdef _WIN32
    closesocket(m_sock);
#else
    close(m_sock);
#endif
    if(m_receiveThread != nullptr)
    {
        m_receiveThread->join();
        delete m_receiveThread;
        m_receiveThread = nullptr;
    }
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

void TcpConnection::stop()
{
    m_isRunning = false;
}
