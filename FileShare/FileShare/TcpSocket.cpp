
#include "TcpSocket.h"

#include <iostream>

TcpSocket::TcpSocket(const SockAddress &addr)
{
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(m_sock < 0)
    {
        m_inError = true;
        m_errorMsg = "Socket creation failed";
    }
    if(bind(m_sock, addr.constAddress(), addr.addressLen()) < 0)
    {
        m_inError = true;
        m_errorMsg = "Socket binding failed";
    }
    m_isRunning = !m_inError;
}

TcpSocket::~TcpSocket()
{
#ifdef _WIN32
    closesocket(m_sock);
#else
    close(m_sock);
#endif
}

bool TcpSocket::start()
{
    if(listen(m_sock, 2) < 0)
    {
        return false;
    }
    while(m_isRunning)
    {
        sockaddr clientAddress;
        memset(&clientAddress, 0, sizeof(sockaddr));
        sockaddrLen clientAddrSize = sizeof(sockaddr);
        int client = accept(m_sock, &clientAddress, &clientAddrSize);
#ifdef _WIN32
        closesocket(client);
#else
        close(client);
#endif
        m_isRunning = false;
    }
    return true;
}

bool TcpSocket::inError() const
{
    return m_inError;
}

const std::string &TcpSocket::errorMsg() const
{
    return m_errorMsg;
}
