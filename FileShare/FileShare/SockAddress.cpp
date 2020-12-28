
#include "SockAddress.h"

SockAddress::SockAddress(const std::string &ipAddress, unsigned short port)
{
    sockaddr_in host;
    memset(&host, 0, sizeof(sockaddr_in));
#ifdef _WIN32
    wchar_t wIpAddress[17];
    unsigned numChars = ipAddress.length();
    mbstowcs_s(&numChars, wIpAddress, ipAddress.c_str(), ipAddress.length());
    wIpAddress[numChars] = '\0';
    InetPton(AF_INET, wIpAddress, &host.sin_addr);
#else
    inet_pton(AF_INET, ipAddress.c_str(), &host.sin_addr);
#endif
    host.sin_family = AF_INET;
    host.sin_port = htons(port);
    m_addr = *(reinterpret_cast<sockaddr *>(&host));
}

SockAddress::SockAddress(const sockaddr &remote)
{
    memcpy(&m_addr, &remote, sizeof(sockaddr));
}

const sockaddr *SockAddress::constAddress() const
{
    return &m_addr;
}

unsigned SockAddress::addressLen() const
{
    return sizeof(sockaddr);
}

const std::string SockAddress::ipAddress()
{
    char buffer[16];
    inet_ntop(AF_INET, &(reinterpret_cast<sockaddr_in *>(&m_addr)->sin_addr), buffer, 16);
    return std::string(buffer);
}
