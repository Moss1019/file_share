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
        OutputMemoryStream stream;
        sockaddr remoteAddr;
        int bytesReceived = m_socket->receiveFrom(stream, &remoteAddr);
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
                auto addrItr = m_addresses.find(identifier);
                if(addrItr == m_addresses.end())
                {
                    m_addresses[identifier] = SockAddress(remote);
                }
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
                break;
            }
            case AddressEventType::FETCH_ALL:
            {
                OutputMemoryStream stream;
                stream.write(static_cast<unsigned>(m_addresses.size()));
                void *identifierBuffer = std::malloc(32);
                void *addressBuffer = std::malloc(16);
                for(auto itr = m_addresses.begin(); itr != m_addresses.end(); ++itr)
                {
                    stream.write(static_cast<unsigned>(itr->first.length()));
                    std::memcpy(identifierBuffer, itr->first.c_str(), itr->first.length());
                    stream.write(identifierBuffer, itr->first.length());
                    stream.write(static_cast<unsigned>(itr->second.ipAddress().length()));
                    std::memcpy(addressBuffer, itr->second.ipAddress().c_str(), itr->second.ipAddress().length());
                    stream.write(addressBuffer, itr->second.ipAddress().length());
                }
                std::free(identifierBuffer);
                std::free(addressBuffer);
                for(auto itr = m_addresses.begin(); itr != m_addresses.end(); ++itr)
                {
                    m_socket->sendTo(stream, itr->second);
                }
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

void AddressServer::printClients(std::ostream &out) const
{
    for (auto itr = m_addresses.begin(); itr != m_addresses.end(); ++itr)
    {
        out << itr->first << std::endl;
    }
}
