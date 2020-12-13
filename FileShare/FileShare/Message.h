#pragma once

#include <string>

#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"

struct Message
{
	unsigned identifierLength;

	unsigned messageLength;

	std::string identifier;

	std::string message;

	void write(OutputMemoryStream &stream)
	{
		stream.write(identifierLength);
		stream.write(messageLength);
		stream.write(identifier.c_str(), identifierLength);
		stream.write(message.c_str(), messageLength);
	}

	void read(InputMemoryStream &stream)
	{
		stream.read(identifierLength);
		stream.read(messageLength);
		stream.read(&identifier, identifierLength);
		stream.read(&message, messageLength);
	}
};
