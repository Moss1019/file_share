

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
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"


int main(int argc, const char * argv[])
{
#ifdef _WIN32
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    bool isRunning = true;

#ifdef _WIN32
    SockAddress hostAddr(L"192.168.1.100", "Windows", 8081);
    UdpSocket socket(hostAddr);
    SockAddress remoteAddr(L"192.168.1.176", "Mac", 8081);
    while(isRunning)
    {
        char buffer[1024];
        unsigned received = socket.receiveFrom(buffer, 1024);
        InputMemoryStream stream(buffer, received);
        CommandInfo info;
        info.read(stream);
        switch(info.type)
        {
            case CMD_DISCONNETED:
            {
                isRunning = false;
            }
            case CMD_CONNECTED:
            {
                std::cout << std::string(static_cast<const char *>(info.data)) << std::endl;
            }
        }
    }
#else
    SockAddress hostAddr("192.168.1.176", "Mac", 8081);
    UdpSocket socket(hostAddr);
    SockAddress remoteAddr("192.168.1.100", "Windows", 8081);
    std::cout << "Waiting\n";
    std::string input;
    while (isRunning)
    {
        std::cin >> input;
        CommandInfo info;
        char inputData[1024];
        info.type = CMD_CONNECTED;
        if (input == "-quit")
        {
            std::cin >> input;
            info.type = CMD_DISCONNETED;
            isRunning = false;
        }
        strcpy(inputData, input.c_str());
        inputData[input.length()] = '\0';
        info.data = reinterpret_cast<void *>(inputData);
        info.dataLength = strlen(inputData);
        OutputMemoryStream stream;
        info.write(stream);
        socket.sendTo(stream.getBufferPtr(), stream.getLength(), remoteAddr);
    }
    

#endif
    socket.closeSocket();

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
