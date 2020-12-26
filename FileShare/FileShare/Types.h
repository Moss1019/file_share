#pragma once
#ifndef socktype

#ifdef _WIN32
#define socktype SOCKET
#else
#define socktype int
#endif

#endif