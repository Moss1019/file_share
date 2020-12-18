
#include "SockAddress.hpp"


SockAddress::SockAddress(const std::string &ipAddress, unsigned short port)
{
    sockaddr_in host;
    memset(&host, 0, sizeof(sockaddr_in));
    inet_pton(AF_INET, ipAddress.c_str(), &host.sin_addr);
    host.sin_family = AF_INET;
    host.sin_port = htons(port);
    m_addr = *(reinterpret_cast<sockaddr *>(&host));
}

const sockaddr *SockAddress::constAddress() const
{
    return &m_addr;
}

unsigned SockAddress::addressLen() const
{
    return sizeof(sockaddr);
}
