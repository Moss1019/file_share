#pragma once

#include <string>
#include <thread>

#include "UpdSocket.h"
#include "InputMemoryStream.h"

class Chat
{
private:
	bool m_isRunning;

	SockAddress *m_address;

	UdpSocket *m_socket;

	std::thread m_receivingThread;

	void (*m_receivedCallback)(InputMemoryStream &stream);

	void receiveHandler();

public:
	Chat(const std::string &identifier, void (*receiveCallback)(InputMemoryStream &stream));

	~Chat();

	void sendMsg(const std::string &msg, SockAddress &remote);

	Chat(const Chat &other) = delete;

	Chat &operator=(const Chat &other) = delete;
};
