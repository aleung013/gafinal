#include "ga_socket.h"
#include <stdlib.h>
#include <stdio.h>

ga_socket::ga_socket()
{
	_sock = 0;
}

ga_socket::~ga_socket()
{
	close();
}

bool ga_socket::open(unsigned int port)
{
	// Create socket
	_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (_sock == -1)
	{
		printf("Could not open socket\n");
		_sock = 0;
		return false;
	}
	// Bind socket
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons((unsigned short)port);
	if (bind(_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		printf("Could not bind socket\n");
		close();
		return false;
	}
	// Set to non-blocking
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	int non_blocking = 1;
	if (fcntl(handle, F_SETFL, O_NONBLOCK, non_blocking) == -1)
	{
		printf("Could not set socket to non-blocking\n");
		return false;
	}
#elif PLATFORM == PLATFORM_WINDOWS
	DWORD non_blocking = 1;
	if (ioctlsocket(_sock,	FIONBIO, &non_blocking) != 0)
	{
		printf("Could not set socket to non-blocking\n");
		return false;
	}
#endif
	// Done setting up packet, return true
	return true;
}

void ga_socket::close()
{
#if PLATFORM == PLATFORM_WINDOWS
	closesocket(_sock);
#else
	close(_sock);
#endif
}

bool ga_socket::is_open() const
{
	return _sock != 0;
}

bool ga_socket::send(const ga_address & dest, const void * data, int size)
{
	// Set up send
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(dest.get_address());
	addr.sin_port = htons(dest.get_port());
	// Send data
	int sent = sendto(_sock, (const char*)data, size, 0, (sockaddr*)&addr, sizeof(sockaddr_in));
	return sent == size;
}

int ga_socket::receive(ga_address & sender, void * data, int size)
{
#if PLATFORM == PLATFORM_WINDOWS
	typedef int socklen_t;
#endif
	sockaddr_in from;
	socklen_t from_length = sizeof(from);
	// Receive data
	int received = recvfrom(_sock, (char*)data, size, 0, (sockaddr*)&from, &from_length);
	// If no data receieved, return 0
	if (received < 0)
	{
		return 0;
	}
	// Set address info
	unsigned int addr = ntohl(from.sin_addr.s_addr);
	unsigned short port = ntohs(from.sin_port);
	sender = ga_address(addr, port);
	return received;
}
