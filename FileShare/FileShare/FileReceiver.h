#pragma once

#include "SockAddress.h"

class FileReceiver
{
private:

public:
	FileReceiver(const SockAddress &host);

	void start();
};

