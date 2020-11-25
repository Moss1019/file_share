#include "InputMemoryStream.h"

#include <memory>
#include <cstring>

InputMemoryStream::InputMemoryStream(char *buffer, int byteCount)
	:m_buffer(buffer), m_head(0), m_capacity(byteCount)
{
}

InputMemoryStream::~InputMemoryStream()
{
	std::free(m_buffer);
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
