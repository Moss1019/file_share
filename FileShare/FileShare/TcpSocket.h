#pragma once


#ifdef _WIN32
#include <WinSock2.h>
#else
#include <unistd.h>
#endif

#include <string>
#include <thread>

#include "Types.h"
#include "SockAddress.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

class TcpSocket
{
private:
    socktype m_sock;
    
    bool m_isRunning = false;
    
    bool m_inError = false;
    
    std::string m_errorMsg;

    std::thread *m_receiveThread = nullptr;

    void (*receiveCallback)(InputMemoryStream &, TcpSocket *);

    void onReceive();
    
public:
    TcpSocket(const SockAddress &remote, void (*receiveCallback)(InputMemoryStream &stream, TcpSocket *client));

    TcpSocket(socktype sock, void(*receiveCallback)(InputMemoryStream &stream, TcpSocket *client));
    
    ~TcpSocket();
    
    bool inError() const;
    
    const std::string &errorMsg() const;

    int sendData(const OutputMemoryStream &stream);
};
