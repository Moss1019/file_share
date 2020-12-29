

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

