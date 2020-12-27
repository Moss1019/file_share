
#include "TcpServer.h"

#ifdef _WIN32

#else
#include <unistd.h>
#endif

#include <memory>

void TcpServer::acceptCallback()
{
    while(m_isRunning)
    {
        sockaddr client;
        memset(&client, 0, sizeof(sockaddr));
        socklen_t clientSize = sizeof(sockaddr);
        int clientSock = accept(m_sock, &client, &clientSize);
        if(m_sock != -1 && clientSock != -1)
        {
            char *buffer = reinterpret_cast<char *>(std::malloc(512));
            int received = recv(clientSock, buffer, 512, 0);
            buffer[received] = '\0';
            send(clientSock, buffer, received, 0);
#ifdef _WIN32
            shutdown(clientSock, SD_BOTH);
            closesocket(clientSock);
#else
            shutdown(clientSock, SHUT_RDWR);
            close(clientSock);
#endif
            std::free(buffer);
        }
        else
        {
            m_isRunning = false;
        }
    }
}

TcpServer::TcpServer(const SockAddress &host)
{
    m_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(bind(m_sock, host.constAddress(), host.addressLen()) < 0)
    {
        m_inError = true;
        m_errorMsg = "Could not bind to ip address";
    }
    if(listen(m_sock, 1) < 0)
    {
        m_inError = true;
        m_errorMsg = "Could not listen on socket";
    }
    m_isRunning = !m_inError;
}

TcpServer::~TcpServer()
{
    stop();
}

bool TcpServer::inError() const
{
    return m_inError;
}

const std::string &TcpServer::errorMsg() const
{
    return m_errorMsg;
}

void TcpServer::start()
{
    m_acceptThread = new std::thread(&TcpServer::acceptCallback, this);
}

void TcpServer::stop()
{
#ifdef _WIN32
    shutdown(m_sock, SD_BOTH);
    closesocket(m_sock);
#else
    shutdown(m_sock, SHUT_RDWR);
    close(m_sock);
#endif
    if(m_acceptThread != nullptr)
    {
        m_acceptThread->join();
        delete m_acceptThread;
        m_acceptThread = nullptr;
    }
}
