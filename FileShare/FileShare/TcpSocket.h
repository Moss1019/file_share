
#pragma once


#include <string>

#ifdef _WIN32
#include <WinSock2.h>

#define sockaddrLen int
#else
#include <unistd.h>

#define sockaddrLen socklen_t
#endif

#include "SockAddress.h"

class TcpSocket
{
private:
    int m_sock;
    
    bool m_isRunning = false;
    
    bool m_inError = false;
    
    std::string m_errorMsg;
    
public:
    TcpSocket(const SockAddress &addr);
    
    ~TcpSocket();
    
    bool start();
    
    bool inError() const;
    
    const std::string &errorMsg() const;
};
