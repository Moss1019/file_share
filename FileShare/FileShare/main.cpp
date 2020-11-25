

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#else

#endif

#include <fcntl.h>

#include <cstring>
#include <iostream>

#include "UpdSocket.h"
#include "SockAddress.h"
#include "CommandInfo.h"
#include "AddressServer.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"


int main(int argc, const char * argv[])
{
#ifdef _WIN32
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    bool isRunning = true;

#ifdef _WIN32
    /*SockAddress hostAddr(L"192.168.1.100", "Windows", 8081);
    UdpSocket socket(hostAddr);
    SockAddress remoteAddr(L"192.168.1.176", "Mac", 8081);
    while(isRunning)
    {
        char *buffer = static_cast<char *>(std::malloc(1024));
        unsigned received = socket.receiveFrom(buffer, 1024);
        InputMemoryStream stream(buffer, received);
        CommandInfo info;
        memset(&info, 0, sizeof(CommandInfo));
        info.read(stream);
        char *data = static_cast<char *>(info.data);
        data[info.dataLength] = '\0';
        std::cout << std::string(data) << std::endl;
        switch(info.type)
        {
            case CMD_DISCONNETED:
            {
                isRunning = false;
            }
        }
    }*/
    AddressServer server(L"192.168.1.100");
    server.startServer();
#else
    SockAddress hostAddr("192.168.1.161", "Pi", 8081);
    UdpSocket socket(hostAddr);
    SockAddress remoteAddr("192.168.1.100", "Windows", 8081);
    std::string input;
    while (isRunning)
    {
        std::cin >> input;
        CommandInfo info;
        char inputData[1024];
        strcpy(inputData, input.c_str());
        inputData[input.length()] = '\0';
        strcpy(info.data, inputData);
        info.dataLength = strlen(inputData);
        info.type = CMD_CONNECTED;
        if (input == "-quit")
        {
            isRunning = false;
            info.type = CMD_DISCONNETED;
        }
        OutputMemoryStream stream;
        info.write(stream);
        socket.sendTo(stream.getBufferPtr(), stream.getLength(), remoteAddr);
    }
    

#endif

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
