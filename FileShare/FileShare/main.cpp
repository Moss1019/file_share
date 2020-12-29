

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#else
#include <memory>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include <fstream>
#include <string>

#include <vector>
#include <cstring>
#include <iostream>

#include "UdpSocket.h"
#include "TcpServer.h"
#include "TcpSocket.h"
#include "SockAddress.h"
#include "AddressEvent.h"
#include "AddressServer.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"


enum class EventType
{
    CONNECTED,
    DISCONNECTED
};

struct Event
{
    EventType type;

    unsigned dataSize = 0;

    void *data = nullptr;
    
    Event()
    {
        
    }

    Event(EventType type)
    {
        this->type = type;
    }

    Event(EventType type, unsigned dataSize, void *data)
    {
        this->type = type;
        this->dataSize = dataSize;
        this->data = data;
    }

    ~Event()
    {
        if (data != nullptr)
        {
            std::free(data);
            data = nullptr;
        }
    }

    void serialize(OutputMemoryStream &stream)
    {
        stream.write(&type, sizeof(EventType));
        stream.write(dataSize);
        if (data != nullptr)
        {
            stream.write(data, dataSize);
        }
    }
    
    void deserialize(InputMemoryStream &stream)
    {
        stream.read(&type, sizeof(EventType));
        stream.read(dataSize);
        if(dataSize > 0)
        {
            data = std::malloc(dataSize);
            stream.read(data, dataSize);
        }
    }
};

int main(int argc, const char * argv[])
{
#ifdef _WIN32
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

#ifdef _WIN32
    std::string ipAddress = "192.168.1.100";
    SockAddress host(ipAddress, 8081);
    std::cout << host.ipAddress() << std::endl;

    UdpSocket updSock(host);

    std::string hostName = "windows";
    void *buffer = std::malloc(hostName.length());
    memcpy(buffer, hostName.c_str(), hostName.length());
    AddressEvent e(AddressEventType::CONNECTED, hostName.length(), buffer);
    OutputMemoryStream stream;
    e.write(stream);

    SockAddress addressHost("192.168.1.176", 8081);
    updSock.sendTo(stream, addressHost);

#else
//    SockAddress host("192.168.1.176", 8081);
//    UdpSocket udpSock(host);
//
//    OutputMemoryStream outStream;
//    sockaddr remote;
//    udpSock.receiveFrom(outStream, &remote);
//    SockAddress remoteHost(remote);
//    std::cout << remoteHost.ipAddress() << std::endl;
//
//
//
//    InputMemoryStream inStream(outStream.getBufferPtr(), outStream.getLength());
//    Event event;
//    event.deserialize(inStream);
//    std::cout << event.dataSize << std::endl;
//    if(event.type == EventType::CONNECTED)
//    {
//        std::cout << std::string(reinterpret_cast<char *>(event.data)) << std::endl;
//    }
    SockAddress host("192.168.1.176", 8081);
    AddressServer server(host);
    server.start();
    
    int x;
    std::cin >> x;
    
    server.stop();
    
#endif

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

//ifdef _WIN32
//   unsigned long mode = blocking ? 0 : 1;
//   return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
//#else
//   int flags = fcntl(fd, F_GETFL, 0);
//   if (flags == -1) return false;
//   flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
//   return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
//#endi
