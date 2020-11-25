#include "SockAddress.h"

sockaddr_in *SockAddress::addressAsSockAddrIn()
{
    return reinterpret_cast<sockaddr_in *>(&m_address);
}

#ifdef _WIN32
SockAddress::SockAddress(const PCWSTR ipAddress, const std::string &identifier, unsigned short port)
    :m_identifier(identifier)
{
    memset(&m_address, 0, sizeof(sockaddr));
    addressAsSockAddrIn()->sin_family = PF_INET;
    addressAsSockAddrIn()->sin_port = htons(port);
    InetPton(AF_INET, ipAddress, &addressAsSockAddrIn()->sin_addr);
}
#else 
SockAddress::SockAddress(const std::string &ipAddress, const std::string &identifier, unsigned short port)
    :m_identifier(identifier)
{
    memset(&m_address, 0, sizeof(sockaddr));
    addressAsSockAddrIn()->sin_family = PF_INET;
    addressAsSockAddrIn()->sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &addressAsSockAddrIn()->sin_addr);
}
#endif

SockAddress::SockAddress(const sockaddr &address, const std::string &identifier)
    :m_identifier(identifier)
{
    memcpy(&m_address, &address, sizeof(sockaddr));
}

const sockaddr *SockAddress::address() const
{
    return &m_address;
}

unsigned SockAddress::addressLen() const
{
    return sizeof(sockaddr);
}

const std::string &SockAddress::identifier() const
{
    return m_identifier;
}

void SockAddress::setIdentifier(const std::string &identifier)
{
    m_identifier = identifier;
}

#ifdef _WIN32
std::string SockAddress::getInetAddress()
{
    char buffer[INET_ADDRSTRLEN];
    InetNtopA(AF_INET, &(addressAsSockAddrIn()->sin_addr), buffer, INET_ADDRSTRLEN);
    return std::string(buffer);
}
#else
std::string SockAddress::getInetAddress()
{
    return std::string(inet_ntoa(addressAsSockAddrIn()->sin_addr));
}
#endif
