#pragma once

#include <memory>

#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"

#define CMD_CONNECTED 1
#define CMD_DISCONNETED 2
#define CMD_GET_ADDRESSES 3

struct CommandInfo
{
	int type;

	int dataLength;

	char data[256];

	void write(OutputMemoryStream &stream) 
	{
		stream.write(type);
		stream.write(dataLength);
		stream.write(data, dataLength);
	}

	void read(InputMemoryStream &stream)
	{
		stream.read(type);
		stream.read(dataLength);
		stream.read(data, dataLength);
	}
};
