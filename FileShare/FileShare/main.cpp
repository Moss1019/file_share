

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#else
#include <memory>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include <fstream>
#include <string>

#include <vector>
#include <cstring>
#include <iostream>

#include "Message.h"
#include "UpdSocket.h"
#include "SockAddress.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

#include "TcpServer.h"
#include "TcpSocket.h"

int main(int argc, const char * argv[])
{
#ifdef _WIN32
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

#ifdef _WIN32
    SockAddress remote("192.168.1.176", 8080);
    TcpSocket socket(remote);
    if (socket.inError())
    {
        std::cout << "IN error " << socket.errorMsg();
    } 
    else
    {
        OutputMemoryStream stream;
        std::ifstream input("C:/Users/mosso/Desktop/Books/Assembly/Assembly Language for x86 Processors, 7th Edition.pdf", std::ifstream::in | std::ifstream::binary);
        char c;
        /*while (true)
        {
            input.get(c);
            stream.write(&c, sizeof(char));
            if (input.eof())
            {
                std::cout << "end of file " << c << " " << stream.getLength();
                break;
            }
        }*/
        //std::cout << std::endl << socket.sendData(stream);
        OutputMemoryStream stream2;
        int received = socket.receiveData(stream2);
        std::cout << "Received " << received << " bytes" << std::endl;
        int x;
        std::cin >> x;
    }
#else
    SockAddress sockAddress("192.168.1.176", 8080); 
    TcpServer server(sockAddress);
    server.start();
    if(server.inError())
    {
        std::cout << server.errorMsg() << std::endl;
    }
    int x;
    std::cin >> x;
    server.stop();
    std::cout << "Done..." << std::endl;
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
