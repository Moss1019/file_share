#pragma once

#ifdef _WIN32
#define sockaddrLen int
#else
#define sockaddrLen socklen_t
#endif

#include "TcpSocket.h"
#include "TcpConnection.h"

void  TcpSocket::listenFunction()
{
    listen(m_sock, 2);
    while(m_isRunning)
    {
        sockaddrLen clientAddrSize = sizeof(sockaddr);
        sockaddr clientAddr;
        memset(&clientAddr, 0, sizeof(sockaddr));
#ifdef _WIN32
        SOCKET clientSock = accept(m_sock, &clientAddr, &clientAddrSize);
#else
        int clientSock = accept(m_sock, &clientAddr, &clientAddrSize);
#endif
        TcpConnection connection(clientSock, m_receiveCallback);
    }
}

TcpSocket::TcpSocket(SockAddress *host, void (*receiveCallback)(InputMemoryStream &stream))
    :m_host(host), m_receiveCallback(receiveCallback)
{
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    bind(m_sock, m_host->address(), m_host->addressLen());
}

void TcpSocket::start()
{
    m_listenThread = new std::thread(&TcpSocket::listenFunction, this);
}

void TcpSocket::stop()
{
    m_isRunning = false;
    if(m_listenThread != nullptr)
    {
        m_listenThread->join();
        delete m_listenThread;
        m_listenThread = nullptr;
    }
}

