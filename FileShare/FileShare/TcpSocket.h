
#pragma once


#include <string>

#ifdef _WIN32
#include <WinSock2.h>
#define sockaddrLen int
#else
#include <unistd.h>

#define sockaddrLen socklen_t
#endif

#include <thread>
#include <vector>

#include "SockAddress.h"
#include "TcpConnection.h"

class TcpSocket
{
private:
    int m_sock;
    
    bool m_isRunning = false;
    
    bool m_inError = false;
    
    std::thread *m_listenThread = nullptr;
    
    std::string m_errorMsg;
    
    std::vector<TcpConnection> m_connections;
    
    void listenCallback();
    
    void (*onReceive)(InputMemoryStream &stream);
    
public:
    TcpSocket(const SockAddress &addr, void (*onReceive)(InputMemoryStream &stream, TcpConnection &client);
    
    ~TcpSocket();
    
    void stop();
    
    bool start();
    
    bool inError() const;
    
    const std::string &errorMsg() const;
};
