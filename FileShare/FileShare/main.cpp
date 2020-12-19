

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
#include <string>

#include <vector>
#include <cstring>
#include <iostream>

#include "Message.h"
#include "UpdSocket.h"
#include "SockAddress.h"
#include "TcpConnection.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

#include "TcpSocket.h"

void onReceive(InputMemoryStream &stream, TcpConnection &client)
{
    std::cout << stream.getRemainingSize() << std::endl;
    char buffer[128];
    int length = stream.getRemainingSize();
    stream.read(buffer, length);
    buffer[length] = '\0';
    std::cout << std::string(buffer);
}

int main(int argc, const char * argv[])
{
#ifdef _WIN32
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

#ifdef _WIN32
    
    SockAddress host("192.168.1.100", 8080);
    SockAddress remote("192.168.1.176", 8080);
    TcpConnection socket(host, remote);
    if (socket.inError())
    {
        std::cout << "IN error " << socket.errorMsg();
    } 
    else
    {
        std::string input;
        std::cin >> input;
        OutputMemoryStream stream;
        stream.write(input.c_str(), input.length());
        std::cout << socket.sendData(stream);
    }
#else
    SockAddress sockAddress("192.168.1.176", 8080); 
    TcpSocket sock(sockAddress, onReceive);
    sock.start();
    int x;
    std::cin >> x;
    sock.stop();
#endif

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

//ifdef _WIN32
//   unsigned long mode = blocking ? 0 : 1;
//   return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
//#else
//   int flags = fcntl(fd, F_GETFL, 0);
//   if (flags == -1) return false;
//   flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
//   return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
//#endi
