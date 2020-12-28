
#include "TcpSocket.h"

TcpSocket::TcpSocket(const SockAddress &remote)
{
    m_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (connect(m_sock, remote.constAddress(), remote.addressLen()) < 0)
    {
        m_inError = true;
        m_errorMsg = "Could not connect to remote host";
    }
    m_isRunning = !m_inError;
}

TcpSocket::TcpSocket(socktype sock)
    :m_sock(sock)
{
    m_isRunning = true;
}

TcpSocket::~TcpSocket()
{
    shutdown(m_sock, SD_BOTH);
    closesocket(m_sock);
}

bool TcpSocket::inError() const
{
    return m_inError;
}

const std::string &TcpSocket::errorMsg() const
{
    return m_errorMsg;
}

int TcpSocket::sendData(const OutputMemoryStream &stream)
{
    int bytesSent = send(m_sock, stream.getBufferPtr(), stream.getLength(), 0);
    return bytesSent;
}

int TcpSocket::receiveData(OutputMemoryStream &stream)
{
    char *buffer = reinterpret_cast<char *>(std::malloc(512));
    bool isReceiving = true;
    int bytesRead = 0;
    int totalRead = 0;
    while (isReceiving)
    {
        bytesRead = recv(m_sock, buffer, 512, 0);
        if (bytesRead <= 0)
        {
            isReceiving = false;
        }
        else
        {
            totalRead += bytesRead;
            stream.write(buffer, bytesRead);
        }
    }
    std::free(buffer);
    return totalRead;
}
