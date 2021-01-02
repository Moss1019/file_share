#pragma once

#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

enum class AddressEventType
{
	DEFAULT,
	CONNECTED,
	DISCONNECTED,
    FETCH_ALL
};

class AddressEvent
{
private:
	AddressEventType m_type = AddressEventType::DEFAULT;

	unsigned m_dataSize = 0;

	void *m_data = nullptr;
public:
	AddressEvent();

	AddressEvent(AddressEventType type);

	AddressEvent(AddressEventType type, unsigned dataSize, void *data);
	
	~AddressEvent();

	AddressEventType type() const;

	unsigned dataSize() const;

	const void *data() const;

	void write(OutputMemoryStream &stream) const;
	
	void read(InputMemoryStream &stream);
};

