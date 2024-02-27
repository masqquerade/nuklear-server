#include <winsock.h>

#pragma once

class IOController
{
public:
	bool static recvAll(SOCKET fd, char* buf, int len);
	bool static sendAll(SOCKET fd, char* buf, int len);
};