#include "TcpConnection.h"

#ifdef _WIN32

#else
#include <unistd.h>
#include <sys/socket.h>
#endif

#include <iostream>

#include "OutputMemoryStream.h"

#define BUFFERSIZE 512

void TcpConnection::receiveFunction()
{
    int bytesReceived = 0;
    char *buffer = reinterpret_cast<char *>(std::malloc(BUFFERSIZE));
    OutputMemoryStream stream;
    while((bytesReceived = recv(m_sock, buffer, BUFFERSIZE, 0)) > 0)
    {
        stream.write(buffer, bytesReceived);
    }
    InputMemoryStream inStream(stream.getBufferPtr(), stream.getLength());
    m_receiveCallback(inStream);
}

TcpConnection::TcpConnection(int sock, void (*receiveCallback)(InputMemoryStream &stream))
    :m_sock(sock), m_receiveCallback{ receiveCallback }
{
    m_receiveThread = new std::thread(&TcpConnection::receiveFunction, this);
}

TcpConnection::TcpConnection(SockAddress *host, SockAddress *remote, void (*receiveCallback)(InputMemoryStream &stream))
    :m_receiveCallback(receiveCallback)
{
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    bind(m_sock, host->address(), host->addressLen());
    std::cout << connect(m_sock, remote->address(), remote->addressLen());
    m_receiveThread = new std::thread(&TcpConnection::receiveFunction, this);
}

TcpConnection::~TcpConnection()
{
    if(m_receiveThread != nullptr)
    {
        m_receiveThread->join();
        delete m_receiveThread;
        m_receiveThread = nullptr;
#ifdef _WIN32
        closesocket(m_sock);
#else

#endif
    }
}

int TcpConnection::sendData(OutputMemoryStream &stream){
    int bytesSent = send(m_sock, stream.getBufferPtr(), stream.getLength(), 0);
    return bytesSent;
}
