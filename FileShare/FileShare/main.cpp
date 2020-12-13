

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <memory>
#endif


#include <vector>
#include <cstring>
#include <iostream>

#include "Chat.h"
#include "Message.h"
#include "UpdSocket.h"
#include "SockAddress.h"
#include "CommandInfo.h"
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



int main(int argc, const char * argv[])
{
#ifdef _WIN32
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

#ifdef _WIN32
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in host;
    memset(&host, 0, sizeof(sockaddr_in));
    InetPton(AF_INET, L"192.168.1.100", &host.sin_addr);
    host.sin_family = AF_INET;
    host.sin_port = htons(8080);
    bind(sock, reinterpret_cast<sockaddr *>(&host));
    
    sockaddr_in remote;
    memset(&remote, 0, sizeof(sockaddr_in));
    InetPton(AF_INET, L"192.168.1.176", &remote.sin_addr);
    remote.sin_family = AF_INET;
    remote.sin_port = htons(8080);
    
    if(connect(sock, reinterpret_cast<sockaddr *>(&remote), sizeof(sockaddrr)) < 0) {
        std::cerr << "Could not connect to remote server" << std::endl;
        close(sock);
        exit(-1);
    }
    
    OutputMemoryStream stream;
    stream.write(10);
    
    int bytesSent = send(sock, stream.getBufferPtr(), stream.getLength, 0);
    std::cout << "sent stuff " << bytesSent << std::endl;
    
    close(sock);
    
#else
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < -1) {
        std::cerr << "Could not create socket" << std::endl;
        exit(-1);
    }
    
    sockaddr_in host;
    memset(&host, 0, sizeof(sockaddr_in));
    inet_pton(AF_INET, "192.168.1.176", &host.sin_addr);
    host.sin_family = AF_INET;
    host.sin_port = htons(8080);
    
    if(bind(sock, reinterpret_cast<sockaddr *>(&host), sizeof(sockaddr)) < 0) {
        std::cerr << "Could not bind socket to host" << std::endl;
        close(sock);
        exit(-1);
    }
    
    sockaddr client;
    memset(&client, 0, sizeof(sockaddr));
    listen(sock, 3);
    
    unsigned sockLen = sizeof(sockaddr);
    int clientSock = accept(sock, &client, &sockLen);
    
    sockaddr_in *clientAsIn = reinterpret_cast<sockaddr_in *>(&client);
    std::cout << "Client connected: " << std::string(inet_ntoa(clientAsIn->sin_addr)) << std::endl;
    std::cout << "this is working" << std::endl;
    
    char *buffer = reinterpret_cast<char *>(std::malloc(128)) ;
    int bytesReceived = recv(sock, buffer, 128, 0);
    InputMemoryStream stream(buffer, bytesReceived);
    int value;
    stream.read(value);
    
    std::cout << "Value recevied " << value << std::endl;
    
    close(clientSock);
    close(sock);
    
#endif

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
