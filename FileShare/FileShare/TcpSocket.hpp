
#pragma once

#define sockaddrLen socklen_t

#include <string>

#include <unistd.h>

#include "SockAddress.hpp"

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
