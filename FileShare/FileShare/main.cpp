

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#else

#endif

#include <thread>
#include <iostream>

#include "SockAddress.h"
#include "UpdSocket.h"

void handleReceiving(bool *isRunning, UdpSocket *socket)
{
    std::cout << "Ready to receive\n";
    char buffer[255];
    std::cout << socket->receiveFrom(buffer, 255);
    while (*isRunning)
    {
        Sleep(1000);
        std::cout << ".\n";
    }
}

int main(int argc, const char * argv[])
{
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SockAddress hostAddr(L"192.168.1.100", "Windows", 8081);
    UdpSocket socket(hostAddr);
    bool isRunning = true;

    std::thread recvThread(handleReceiving, &isRunning, &socket);

    SockAddress remoteAddr(L"192.168.1.176", "Mac", 8081);

    std::string input;
    while (isRunning)
    {
        std::cin >> input;
        if (input == "-quit")
        {
            isRunning = false;
            break;
        }
        socket.sendTo(input.c_str(), input.length(), remoteAddr);
    }

    socket.closeSocket();

    recvThread.join();

    WSACleanup();
    return 0;
}
