#include "OutputMemoryStream.h"

#include <memory>
#include <cstring>
#include <algorithm>

void OutputMemoryStream::reallocate(int newLength)
{
	m_buffer = static_cast<char *>(std::realloc(m_buffer, newLength));
	m_capacity = newLength;
}

OutputMemoryStream::OutputMemoryStream()
	:m_buffer(nullptr), m_head(0), m_capacity(0)
{
	reallocate(32);
}

OutputMemoryStream::~OutputMemoryStream()
{
	std::free(m_buffer);
}

const char *OutputMemoryStream::getBufferPtr() const
{
	return m_buffer;
}

int OutputMemoryStream::getLength() const
{
	return m_head;
}

void OutputMemoryStream::write(const void *data, unsigned byteCount)
{
	unsigned resultHead = m_head + static_cast<unsigned>(byteCount);
	if (resultHead > m_capacity)
	{
		reallocate(std::max(m_capacity * 2, resultHead));
	}
	std::memcpy(m_buffer + m_head, data, byteCount);
	m_head = resultHead;
}

void OutputMemoryStream::write(int data)
{
	write(&data, sizeof(int));
}

void OutputMemoryStream::write(unsigned data)
{
	write(&data, sizeof(unsigned));
}
