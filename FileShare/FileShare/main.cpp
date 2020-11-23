

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

    std::cout << sizeof(CommandInfo) << std::endl;

    SockAddress hostAddr(L"192.168.1.100", "Windows", 8081);
    UdpSocket socket(hostAddr);
    bool isRunning = true;

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
        CommandInfo info;
        char inputData[1024];
        strcpy(inputData, input.c_str());
        inputData[input.length()] = '\0';
        info.data = reinterpret_cast<void *>(inputData);
        info.dataLength = strlen(inputData);
        info.type = CMD_CONNECTED;
        socket.sendTo((const char *)&info, input.length() + 8, remoteAddr);
    }

    socket.closeSocket();

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
