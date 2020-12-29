

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

int main(int argc, const char * argv[])
{
#ifdef _WIN32
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

#ifdef _WIN32
    SockAddress host("192.168.1.100", 8081);
    AddressServer server(host);
    server.start();
    
    int x;
    std::cin >> x;
    
    server.stop();
    

#else
    std::string ipAddress = "192.168.1.176";
    SockAddress host(ipAddress, 8081);

    UdpSocket updSock(host);
    SockAddress addressHost("192.168.1.100", 8081);

    std::string hostName = "mac";
    void *buffer = std::malloc(hostName.length());
    memcpy(buffer, hostName.c_str(), hostName.length());

    AddressEvent e1(AddressEventType::CONNECTED, hostName.length(), buffer);
    OutputMemoryStream stream1;
    e1.write(stream1);

    updSock.sendTo(stream1, addressHost);

    AddressEvent e2(AddressEventType::DISCONNECTED, hostName.length(), buffer);
    OutputMemoryStream stream2;
    e2.write(stream2);

    updSock.sendTo(stream2, addressHost);

    AddressEvent e3(AddressEventType::DEFAULT, hostName.length(), buffer);
    OutputMemoryStream stream3;
    e3.write(stream3);

    std::cout << updSock.sendTo(stream3, addressHost) << " sent\n";

    std::free(buffer);
    
    std::cout << "Done" << std::endl;
#endif

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

