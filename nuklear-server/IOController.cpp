#include "IOController.h"
#include <winsock.h>

bool IOController::recvAll(SOCKET fd, char* buf, int len)
{
	while (len > 0)
	{
		int bytes = recv(fd, buf, len, 0);
		if (bytes == SOCKET_ERROR)
		{
			return false;
		}
		else if (bytes == 0)
		{
			return false;
		}

		if (bytes >= len)
		{
			return true;
		}

		buf += bytes;
		len -= bytes;
	}

	return true;
}

bool IOController::sendAll(SOCKET fd, char* buf, int len)
{
	while (len > 0)
	{
		int bytes = send(fd, buf, len, 0);
		if (bytes == SOCKET_ERROR)
		{
			return false;
		}

		if (bytes >= len)
		{
			return true;
		}

		buf += bytes;
		len -= bytes;
	}

	return true;
}