#include "Protocol.h"
#include <iostream>
#include <winsock.h>
#include "IOController.h"

#define LEN_SIZE 4
#define MAX_MSG 256

bool Protocol::recv(SOCKET fd)
{
	char rbuf[LEN_SIZE];
	int msg_len;

	if (!IOController::recvAll(fd, rbuf, LEN_SIZE))
	{
		std::cout << "recvAll() error" << std::endl;
		return false;
	}

	std::memcpy(&msg_len, rbuf, LEN_SIZE);

	if (msg_len > MAX_MSG)
	{
		std::cout << "Message is too long" << std::endl;
		return false;
	}

	if (!IOController::recvAll(fd, &rbuf[LEN_SIZE], msg_len))
	{
		std::cout << "recvAll() data error" << std::endl;
		return false;
	}

	return true;
}

/*
	WBUF STRUCTURE:

	[0...LEN_SIZE]: size of the message (msg_len)
	[LEN_SIZE...msg_len]: message
*/

bool Protocol::send(SOCKET fd, char *wbuf)
{
	int msg_len;
	memcpy(&msg_len, wbuf, LEN_SIZE);

	if (!IOController::sendAll(fd, wbuf, LEN_SIZE))
	{
		std::cout << "sendAll() error" << std::endl;
		return false;
	}

	if (!IOController::sendAll(fd, &wbuf[LEN_SIZE], msg_len))
	{
		std::cout << "sendAll() data error" << std::endl;
		return false;
	}

	return true;
}