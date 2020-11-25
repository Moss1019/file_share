

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#else

#endif

#include <fcntl.h>

#include <iostream>

#include "SockAddress.h"
#include "UpdSocket.h"
#include "CommandInfo.h"


int main(int argc, const char * argv[])
{
#ifdef _WIN32
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    int value = 90;
    auto f = [value](int signal) { std::cout << value << "Hello lambda\n";  };

    SockAddress hostAddr("192.168.1.176", "Host", 8081);
    UdpSocket socket(hostAddr);
    bool isRunning = false;

    SockAddress remoteAddr("192.168.1.100", "Remote", 8081);

    std::string input;
    while (isRunning)
    {
        std::cin >> input;
        if (input == "-quit")
        {
            isRunning = false;
            break;
        }
        CommandInfo info;
        char inputData[1024];
        strcpy(inputData, input.c_str());
        inputData[input.length()] = '\0';
        info.data = reinterpret_cast<void *>(inputData);
        info.dataLength = strlen(inputData);
        info.type = CMD_CONNECTED;
        socket.sendTo((const char *)&info, input.length() + 8, remoteAddr);
    }
    
    char buffer[1024];
    int received = socket.receiveFrom(buffer, 1024);
    std::cout << received << std::endl;
    CommandInfo info;
    socket.closeSocket();

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
