#include "AddressServer.h"

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

#include <string>

#include "AddressEvent.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

#include <iostream>

AddressServer::AddressServer(const SockAddress &host)
{
    m_socket = new UdpSocket(host);
    m_isRunning = true;
}

AddressServer::~AddressServer()
{
    stop();
}

void AddressServer::run()
{
    while(m_isRunning)
    {
        std::cout << "Wainting\n";
        OutputMemoryStream stream;
        sockaddr remoteAddr;
        int bytesReceived = m_socket->receiveFrom(stream, &remoteAddr);
        std::cout << "Received " << bytesReceived;
        if(bytesReceived <= 0)
        {
            continue;
        }
        SockAddress remote(remoteAddr);
        InputMemoryStream inStream(stream.getBufferPtr(), stream.getLength());
        AddressEvent addrEvent;
        addrEvent.read(inStream);
        switch(addrEvent.type())
        {
            case AddressEventType::CONNECTED:
            {
                char *buffer = reinterpret_cast<char *>(std::malloc(addrEvent.dataSize()));
                std::memcpy(buffer, addrEvent.data(), addrEvent.dataSize());
                std::string identifier(buffer);
                std::free(buffer);
                m_addresses[identifier] = SockAddress(remote);
                
                std::cout << "Connected " << identifier << std::endl;
                
                break;
            }
            case AddressEventType::DISCONNECTED:
            {
                char *buffer = reinterpret_cast<char *>(std::malloc(addrEvent.dataSize()));
                std::memcpy(buffer, addrEvent.data(), addrEvent.dataSize());
                std::string identifier(buffer);
                std::free(buffer);
                auto addrIter = m_addresses.find(identifier);
                if(addrIter != m_addresses.end())
                {
                    m_addresses.erase(addrIter);
                }
                
                std::cout << "Disconnected " << identifier << std::endl;
                
                break;
            }
            default:
            {
                std::cout << "Strange" << std::endl;
                m_isRunning = false;
                break;
            }
        }
    }
}

void AddressServer::start()
{
    m_runThread = new std::thread(&AddressServer::run, this);
}

void AddressServer::stop()
{
    m_isRunning = false;
    if(m_socket != nullptr)
    {
        delete m_socket;
        m_socket = nullptr;
    }
    if(m_runThread != nullptr)
    {
        m_runThread->join();
        delete m_runThread;
        m_runThread = nullptr;
    }
}
