#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

#include <thread>

#include "SockAddress.h"
#include "InputMemoryStream.h"

class TcpSocket
{
private:
#ifdef _WIN32
    SOCKET m_sock;
#else
    int m_sock;
#endif
    
    bool m_isRunning = false;
    
    SockAddress *m_host = nullptr;
    
    std::thread *m_listenThread = nullptr;
    
    void (*m_receiveCallback)(InputMemoryStream &stream);
    
    void listenFunction();

public:
    TcpSocket(SockAddress *host, void (*receiveCallback)(InputMemoryStream &stream));
    
    void start();
    
    void stop();
};

