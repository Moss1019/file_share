#pragma once

#define CMD_CONNECTED 1
#define CMD_DISCONNETED 2
#define CMD_GET_ADDRESSES 3

struct CommandInfo
{
	int type;

	size_t dataLength;

	void *data;
};