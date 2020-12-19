
#include "TcpSocket.h"

#include <errno.h>
#include <fcntl.h>

#include <chrono>

#include <iostream>
#include <memory>

#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

void TcpSocket::listenCallback()
{
    while(m_isRunning)
    {
        std::cout << "Waiting\n";
        sockaddr clientAddress;
        memset(&clientAddress, 0, sizeof(sockaddr));
        sockaddrLen clientAddrSize = sizeof(sockaddr);
        int client = accept(m_sock, &clientAddress, &clientAddrSize);
#ifdef _WIN32
        
#else
        if(errno == EWOULDBLOCK)
#endif
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        else if(client > 0)
        {
            TcpConnection newClient(client, onReceive);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            std::cout << "Some other shit\n";
        }
    }
}

TcpSocket::TcpSocket(const SockAddress &addr, void (*onReceive)(InputMemoryStream &stream))
:onReceive(onReceive)
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
    stop();
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
    m_listenThread = new std::thread(&TcpSocket::listenCallback, this);
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
