#include <winsock.h>

#pragma once

class Protocol
{
public:
	bool static recv(SOCKET fd);
	bool static send(SOCKET fd, char* wbuf);
};