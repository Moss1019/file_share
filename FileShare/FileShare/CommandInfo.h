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

	std::string data;

	void write(OutputMemoryStream &stream) 
	{
		stream.write(type);
		stream.write(static_cast<unsigned>(data.length()));
		stream.write(data.c_str(), data.length());
	}

	void read(InputMemoryStream &stream)
	{
		stream.read(type);
        int dataLength;
        stream.read(dataLength);
        char dataBuffer[256];
		stream.read(dataBuffer, dataLength);
        dataBuffer[dataLength] = '\0';
        data = std::string(dataBuffer);
	}
};
