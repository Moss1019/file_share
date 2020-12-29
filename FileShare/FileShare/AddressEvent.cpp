#include "AddressEvent.h"

#include <memory>

AddressEvent::AddressEvent()
{
}

AddressEvent::AddressEvent(AddressEventType type)
	:m_type(type)
{
}

AddressEvent::AddressEvent(AddressEventType type, unsigned dataSize, void *data)
	:m_type(type), m_dataSize(dataSize)
{
	if (dataSize > 0)
	{
		m_data = std::malloc(m_dataSize);
		std::memcpy(m_data, data, m_dataSize);
	}
}

AddressEvent::~AddressEvent()
{
	if (m_data != nullptr)
	{
		std::free(m_data);
		m_data = nullptr;
	}
}

AddressEventType AddressEvent::type() const
{
	return m_type;
}

unsigned AddressEvent::dataSize() const
{
	return m_dataSize;
}

const void *AddressEvent::data() const
{
	return m_data;
}

void AddressEvent::write(OutputMemoryStream &stream) const
{
	stream.write(&m_type, sizeof(AddressEventType));
	stream.write(m_dataSize);
	if (m_dataSize > 0)
	{
		stream.write(m_data, m_dataSize);
	}
}

void AddressEvent::read(InputMemoryStream &stream)
{
	stream.read(&m_type, sizeof(AddressEventType));
	stream.read(m_dataSize);
	if (m_dataSize > 0)
	{
		m_data = std::malloc(m_dataSize);
		stream.read(m_data, m_dataSize);
	}
}
