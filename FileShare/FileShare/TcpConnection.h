#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else

#endif

#include <thread>

#include "SockAddress.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

class TcpConnection
{
private:
#ifdef _WIN32
    SOCKET m_sock;
#else
    int m_sock;
#endif
    
    std::thread *m_receiveThread = nullptr;
    
    void (*m_receiveCallback)(InputMemoryStream &stream);
    
    void receiveFunction();

public:
    TcpConnection(int sock, void (*receiveCallback)(InputMemoryStream &stream));
    
    TcpConnection(SockAddress *host, SockAddress *remote, void (*receiveCallback)(InputMemoryStream &stream));
    
    ~TcpConnection();

    int sendData(OutputMemoryStream &stream);
};

