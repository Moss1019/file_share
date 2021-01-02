

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
    SockAddress addressServer("192.168.1.176", 8081);
    SockAddress udpHost("192.168.1.100", 8080);
    UdpSocket udpSock(udpHost);

    std::string hostName = "windows";
    void *buffer = std::malloc(hostName.length());
    std::memcpy(buffer, hostName.c_str(), hostName.length());

    AddressEvent connectEv(AddressEventType::CONNECTED, hostName.length(), buffer);
    OutputMemoryStream connectEvStream;
    connectEv.write(connectEvStream);

    AddressEvent disconnectEv(AddressEventType::DISCONNECTED, hostName.length(), buffer);
    OutputMemoryStream disconnectEvStream;
    disconnectEv.write(disconnectEvStream);

    AddressEvent fetchAllEv(AddressEventType::FETCH_ALL);
    OutputMemoryStream fetchAllEvStream;
    fetchAllEv.write(fetchAllEvStream);

    udpSock.sendTo(connectEvStream, addressServer);
    bool isRunning = true;
    while (isRunning)
    {
        int choice;
        std::cout << "0. quit" << std::endl 
            << "1. disconnect" << std::endl 
            << "2. connect" << std::endl
            << "3. get addresses";
        std::cout << " -> ";
        std::cin >> choice;
        switch (choice)
        {
        case 0:
        {
            isRunning = false;
            break;
        }
        case 1:
        {
            udpSock.sendTo(disconnectEvStream, addressServer);
            break;
        }
        case 2:
        {
            udpSock.sendTo(connectEvStream, addressServer);
            break;
        }
        case 3:
        {
            udpSock.sendTo(fetchAllEvStream, addressServer);
            OutputMemoryStream addressStream;
            int received = udpSock.receiveFrom(addressStream);
            std::cout << "received " << received << " bytes" << std::endl;
            InputMemoryStream addressInStream(addressStream.getBufferPtr(), addressStream.getLength());
            unsigned numAddresses = 0;
            addressInStream.read(numAddresses);
            for (int i = 0; i < numAddresses; ++i)
            {
                unsigned identifierSize;
                addressInStream.read(identifierSize);
                char *idendifierBuffer = reinterpret_cast<char *>(std::malloc(identifierSize));
                addressInStream.read(idendifierBuffer, identifierSize);
                std::cout << identifierSize << " " << std::string(idendifierBuffer) << " ";
                std::free(idendifierBuffer);
                unsigned addressSize;
                addressInStream.read(addressSize);
                char *addressBuffer = reinterpret_cast<char *>(std::malloc(addressSize));
                addressInStream.read(addressBuffer, addressSize);
                std::cout << addressSize << " " << std::string(addressBuffer) << std::endl;
                std::free(addressBuffer);
            }

            break;
        }
        }
    }
    udpSock.sendTo(disconnectEvStream, addressServer);
#else
    SockAddress udpHost("192.168.1.176", 8081);
    SockAddress tcpHost("192.168.1.176", 8080);

    AddressServer server(udpHost);
    server.start();

    bool isRunning = true;

    while (isRunning)
    {
        int choice;
        std::cout << "0. quit" << std::endl << "1. show clients" << " -> ";
        std::cin >> choice;
        switch (choice)
        {
        case 0:
        {
            isRunning = false;
            break;
        }
        case 1:
        {
            std::cout << "Clients: " << std::endl;
        }
        }
    }

    server.stop();
#endif

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

