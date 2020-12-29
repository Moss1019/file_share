#include "AddressServer.h"

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

#include "OutputMemoryStream.h"

#include <iostream>

AddressServer::AddressServer(const SockAddress &host)
{
    m_socket = new UdpSocket(host);
}

AddressServer::~AddressServer()
{
    stop();
}

void AddressServer::start()
{
    std::cout << "Waiting " << std::endl;
    
    OutputMemoryStream stream;
    sockaddr remoteAddr;
    m_socket->receiveFrom(stream, &remoteAddr);
    SockAddress remote(remoteAddr);
    
    std::cout << "Connected " << remote.ipAddress() << std::endl;
}

void AddressServer::stop()
{
    m_isRunning = false;
    if(m_socket != nullptr)
    {
        delete m_socket;
        m_socket = nullptr;
    }
}
