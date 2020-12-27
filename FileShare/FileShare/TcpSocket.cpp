
#include "TcpSocket.h"


void TcpSocket::onReceive()
{
    char *buffer = reinterpret_cast<char *>(std::malloc(512));
    OutputMemoryStream stream;
    while (m_isRunning)
    {
        bool isReceiving = true;
        while (isReceiving)
        {
            int bytesRead = recv(m_sock, buffer, 512, 0);
            if (bytesRead == 0)
            {
                isReceiving = false;
                InputMemoryStream inputStream(stream.getBufferPtr(), stream.getLength());
                receiveCallback(inputStream, this);
            }
            else if (bytesRead == -1)
            {
                isReceiving = false;
                m_isRunning = false;
            }
            else
            {
                stream.write(buffer, bytesRead);
            }
        }
    }
    std::free(buffer);
}

TcpSocket::TcpSocket(const SockAddress &remote, void(*receiveCallback)(InputMemoryStream &stream, TcpSocket *client))
    :receiveCallback(receiveCallback)
{
    m_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (connect(m_sock, remote.constAddress(), remote.addressLen()) < 0)
    {
        m_inError = true;
        m_errorMsg = "Could not connect to remote host";
    }
    m_receiveThread = new std::thread(&TcpSocket::onReceive, this);
    m_isRunning = !m_inError;
}

TcpSocket::TcpSocket(socktype sock, void(*receiveCallback)(InputMemoryStream &stream, TcpSocket *client))
    :m_sock(sock), receiveCallback(receiveCallback)
{
    m_isRunning = true;
}

TcpSocket::~TcpSocket()
{
    shutdown(m_sock, SD_BOTH);
    closesocket(m_sock);
    if (m_receiveThread != nullptr)
    {
        m_receiveThread->join();
        delete m_receiveThread;
        m_receiveThread = nullptr;
    }
}

bool TcpSocket::inError() const
{
    return m_inError;
}

const std::string &TcpSocket::errorMsg() const
{
    return m_errorMsg;
}

int TcpSocket::sendData(const OutputMemoryStream &stream)
{
    char buffer[] = "hello";
    int bytesSent = send(m_sock, buffer, 6, 0);
    return bytesSent;
}
