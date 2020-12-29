#pragma once

#include <map>
#include <thread>

#include "UdpSocket.h"
#include "SockAddress.h"

class AddressServer
{
private:
    bool m_isRunning;
    
    UdpSocket *m_socket;
    
    std::thread *m_runThread = nullptr;
    
    std::map<std::string, SockAddress> m_addresses;
    
    void run();

public:
    AddressServer(const SockAddress &host);
    
    ~AddressServer();
    
    void start();
    
    void stop();
};

