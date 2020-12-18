
#pragma once

#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>

class SockAddress
{
private:
    sockaddr m_addr;
public:
    SockAddress(const std::string &ipAddress, unsigned short port);
    
    const sockaddr *constAddress() const;
    
    unsigned addressLen() const;
};
