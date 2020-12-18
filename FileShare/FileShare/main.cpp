

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
    /*SockAddress host(L"192.168.1.100", "windows", 8080);
    SockAddress remote(L"192.168.1.176", "mac", 8080);
    TcpConnection client(&remote, &host, receivedData);
    OutputMemoryStream stream;
    stream.write(10);
    std::cout << client.sendData(stream) << std::endl;*/

    /*SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in host;
    memset(&host, 0, sizeof(sockaddr_in));
    InetPton(AF_INET, L"192.168.1.100", &host.sin_addr);
    host.sin_family = AF_INET;
    host.sin_port = htons(8080);
    std::cout << bind(sock, reinterpret_cast<sockaddr *>(&host), sizeof(sockaddr)) << std::endl;

    sockaddr_in remote;
    memset(&remote, 0, sizeof(sockaddr_in));
    InetPton(AF_INET, L"192.168.1.176", &remote.sin_addr);
    remote.sin_family = AF_INET;
    remote.sin_port = htons(8080);

    int connectResult = 0;

    if ((connectResult = connect(sock, reinterpret_cast<sockaddr *>(&remote), sizeof(sockaddr))) < 0) 
    {
        std::cerr << "Could not connect to remote server " << connectResult << std::endl;
        closesocket(sock);
        exit(-1);
    }

    std::ifstream inFileStream;
    inFileStream.open("C:/Users/mosso/Desktop/Projects/file_share/FileShare/Debug/file");
    char *buffer = reinterpret_cast<char *>(std::malloc(128));
    OutputMemoryStream stream;
    int bytesRead = 0;
    while (inFileStream.read(buffer + bytesRead++, 1))
    {

        if (bytesRead == 128)
        {
            stream.write(buffer, bytesRead);
            bytesRead = 0;
        }
    }

    stream.write(buffer, bytesRead);
    std::free(buffer);

    int bytesSent = send(sock, stream.getBufferPtr(), stream.getLength(), 0);
    std::cout << "sent stuff " << bytesSent << std::endl;

    closesocket(sock);*/

    SockAddress host(L"192.169.1.100", "windows", 8080);
    TcpSocket socket(&host, receivedData);
    socket.start();
    socket.stop();
    std::cout << "Done" << std::endl;
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
