

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
