#include "InputMemoryStream.h"

#include <memory>
#include <cstring>

#include <iostream>

InputMemoryStream::InputMemoryStream(char *buffer, int byteCount)
	:m_head(0), m_capacity(byteCount)
{
    m_buffer = reinterpret_cast<char *>(std::malloc(byteCount));
    std::memcpy(m_buffer, buffer, byteCount);
}

InputMemoryStream::InputMemoryStream(const char *buffer, int byteCount)
	: m_head(0), m_capacity(byteCount)
{
    m_buffer = reinterpret_cast<char *>(std::malloc(byteCount));
	std::memcpy(m_buffer, buffer, byteCount);
}

InputMemoryStream::~InputMemoryStream()
{
	if (m_buffer != nullptr)
	{
		std::free(m_buffer);
		m_buffer = nullptr;
	}
}

unsigned InputMemoryStream::getRemainingSize() const
{
	return m_capacity - m_head;
}

void InputMemoryStream::read(void *data, unsigned byteCount)
{
	std::memcpy(data, m_buffer + m_head, byteCount);
	m_head += byteCount;
}

void InputMemoryStream::read(int &data)
{
	read(&data, sizeof(int));
}

void InputMemoryStream::read(unsigned &data)
{
	read(&data, sizeof(unsigned));
}
