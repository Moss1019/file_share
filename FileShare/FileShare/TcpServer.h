#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include <thread>

#include "Types.h"
#include "SockAddress.h"

class TcpServer
{
private:
    bool m_isRunning;
    
    bool m_inError;
    
    socktype m_sock;
    
    std::thread *m_acceptThread = nullptr;
    
    std::string m_errorMsg;
    
    void acceptCallback();
    
public:
    TcpServer(const SockAddress &host);
    
    ~TcpServer();
    
    bool inError() const;
    
    const std::string &errorMsg() const;
    
    void start();
    
    void stop();
};
