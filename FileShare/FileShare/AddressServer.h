#pragma once

#include "UdpSocket.h"
#include "SockAddress.h"

class AddressServer
{
private:
    bool m_isRunning;
    
    UdpSocket *m_socket;

public:
    AddressServer(const SockAddress &host);
    
    ~AddressServer();
    
    void start();
    
    void stop();
};

