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
    
    bool m_inError = false;
    
    std::string m_errorMsg;

public:
    TcpSocket(const SockAddress &remote);

    TcpSocket(socktype sock);
    
    ~TcpSocket();
    
    bool inError() const;
    
    const std::string &errorMsg() const;

    int sendData(const OutputMemoryStream &stream);

    int receiveData(OutputMemoryStream &stream);
};
