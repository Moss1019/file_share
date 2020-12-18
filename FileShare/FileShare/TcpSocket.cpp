#pragma once

#ifdef _WIN32
#define sockaddrLen int
#include <WinSock2.h>
#else
#define sockaddrLen socklen_t
#endif

#include <iostream>

#include "TcpSocket.h"
#include "TcpConnection.h"

void  TcpSocket::listenFunction()
{
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
    sockaddr_in shost;
    memset(&shost, 0, sizeof(sockaddr_in));
    InetPton(AF_INET, L"192.168.1.100", &shost.sin_addr);
    shost.sin_family = AF_INET;
    shost.sin_port = htons(8080);
    std::cout << bind(m_sock, host->address(), sizeof(sockaddr)) << std::endl;
    //std::cout << bind(m_sock, m_host->address(), m_host->addressLen()) << std::endl;
    std::cout << listen(m_sock, 2) << std::endl;
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

