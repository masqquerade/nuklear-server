#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <iostream>

#define DEFAULT_PORT "12345"
#define BACKLOG 20

int main()
{
	u_long mode = 1;

	struct addrinfo* res = NULL;
	struct addrinfo hints;
	char rbuf[128];
	char wbuf[128] = "hello";

	SOCKET listen_fd = INVALID_SOCKET, client_fd = INVALID_ATOM;
	WSADATA wsadata;
	fd_set rfds;
	fd_set master;
	fd_set wfds;

	FD_ZERO(&rfds);
	FD_ZERO(&master);
	FD_ZERO(&wfds);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		std::cout << "wsastartup() error" << std::endl;
		return -1;
	}

	if (getaddrinfo(NULL, DEFAULT_PORT, &hints, &res) != 0)
	{
		std::cout << "getaddrinfo() error" << std::endl;
		return -1;
	}

	listen_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (listen_fd == INVALID_SOCKET)
	{
		std::cout << "socket() error" << std::endl;
		return -1;
	}

	if (bind(listen_fd, res->ai_addr, res->ai_addrlen) == SOCKET_ERROR)
	{
		std::cout << "bind() error" << std::endl;
		return -1;
	}

	if (listen(listen_fd, BACKLOG) == SOCKET_ERROR)
	{
		std::cout << "listen() error" << std::endl;
		return -1;
	}

	ioctlsocket(listen_fd, FIONBIO | FIONREAD, &mode);

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	
	FD_SET(listen_fd, &rfds);

	while (true)
	{
		if (select(0, &rfds, &wfds, NULL, NULL) == SOCKET_ERROR)
		{
			std::cout << "select() error: " << WSAGetLastError() << std::endl;
			return -1;
		}

		for (int i = 0; i < rfds.fd_count; i++)
		{
			if (rfds.fd_array[i] == listen_fd)
			{
				client_fd = accept(listen_fd, NULL, NULL);
				if (client_fd == INVALID_SOCKET)
				{
					std::cout << "accept client error" << std::endl;
					return -1;
				}

				FD_SET(client_fd ,&rfds);
				FD_SET(client_fd, &wfds);

				std::cout << "New connection" << std::endl;
			}
			else
			{
				int bytes = recv(rfds.fd_array[i], rbuf, sizeof(rbuf), 0);
				if (bytes == SOCKET_ERROR)
				{
					std::cout << "recv error" << std::endl;
					return -1;
				}
				else if (bytes == 0)
				{
					std::cout << "no more data" << std::endl;
				}

				for (int i = 0; i < 1; i++)
				{
					std::cout << rbuf[0] << std::endl;
				}

				FD_CLR(rfds.fd_array[i], &rfds);

				if (!FD_ISSET(rfds.fd_array[i], &wfds))
				{
					shutdown(rfds.fd_array[i], 1);
				}
			}
		}

		for (int i = 0; i < wfds.fd_count; i++)
		{
			int bytes = send(wfds.fd_array[i], wbuf, sizeof(wbuf), 0);
			if (bytes == SOCKET_ERROR)
			{
				std::cout << "send error: "<< WSAGetLastError() << std::endl;
				return -1;
			}

			FD_CLR(wfds.fd_array[i], &wfds);

			if (!FD_ISSET(wfds.fd_array[i], &rfds))
			{
				shutdown(rfds.fd_array[i], 1);
			}
		}
	}

	return 0;
}