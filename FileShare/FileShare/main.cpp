

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <memory>
#endif

#include <fstream>

#include <vector>
#include <cstring>
#include <iostream>

#include "Chat.h"
#include "Message.h"
#include "UpdSocket.h"
#include "TcpSocket.h"
#include "SockAddress.h"
#include "CommandInfo.h"
#include "TcpConnection.h"
#include "AddressRecord.h"
#include "AddressServer.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"


void receivedMsg(InputMemoryStream &stream)
{
    int type;
    stream.read(type);
    switch(type)
    {
        case 11:
        {
            std::cout << "Got some addresses\n";
            break;
        }
        case 12:
        {
            std::cout << "Got a msg\n";
            break;
        }
    }
}

bool isRunning = false;

void receivedData(InputMemoryStream &stream)
{
    std::cout << "Got some data\n";
}

int main(int argc, const char * argv[])
{
#ifdef _WIN32
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

#ifdef _WIN32
    SockAddress host(L"192.168.1.100", "windows", 8080);
    SockAddress remote(L"192.168.1.176", "mac", 8080);
    TcpConnection client(&host, &remote, receivedData);
    OutputMemoryStream stream;
    stream.write(10);
    client.sendData(stream);
    int x;
    std::cin >> x;
#else
    SockAddress host("192.169.1.176", "mac", 8080);
    TcpSocket socket(&host, receivedData);
    socket.start();
    int x;
    std::cin >> x;
    socket.stop();
    std::cout << "Done" << std::endl;
#endif

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
