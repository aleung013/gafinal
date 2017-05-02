#pragma once

// Platform Detection
#define PLATFORM_WINDOWS 1
#define PLATFORM_MAC 2
#define PLATFORM_UNIX 3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS
#include <winsock2.h>
#elif PLATFORM == PLATFORM_MAC || PLATFORM_UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#endif

#if PLATFORM == PLATFORM_WINDOWS
#pragma comment(lib, "wsock32.lib")
#endif

#define MAX_BUFFER 256

#include "ga_address.h"
// Socket header from http://gafferongames.com/networking-for-game-programmers/sending-and-receiving-packets/
class ga_socket
{
public:
	ga_socket();
	~ga_socket();
	bool open(unsigned int port);
	void close();
	bool is_open() const;
	bool send(const ga_address & dest, const void* data, int size);
	int receive(ga_address & sender, void * data, int size);
private:
	int _sock;
};