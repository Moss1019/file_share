#pragma once

#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"

struct AddressRecord
{
	unsigned ipAddressLength;

	unsigned identifierLength;

	char ipAddress[16];

	char identifier[256];

	void write(OutputMemoryStream &stream)
	{
		stream.write(ipAddressLength);
		stream.write(identifierLength);
		stream.write(ipAddress, ipAddressLength);
		stream.write(identifier, identifierLength);
	}

	void read(InputMemoryStream &stream)
	{
		stream.read(ipAddressLength);
		stream.read(identifierLength);
		stream.read(ipAddress, ipAddressLength);
		stream.read(identifier, identifierLength);
	}
};