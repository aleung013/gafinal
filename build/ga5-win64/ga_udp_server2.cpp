#include "ga_udp_server.h"

ga_udp_server::ga_udp_server(short port)
{
	initialize_sockets();
	_socket = new ga_socket();
	_socket->open(port);
}

ga_udp_server::~ga_udp_server()
{
	delete _socket;
	shutdown_sockets();
}

bool ga_udp_server::initialize_sockets()
{
#if PLATFORM == PLATFORM_WINDOWS
	WSADATA WsaData;
	return WSAStartup(MAKEWORD(2, 2),
		&WsaData)
		== NO_ERROR;
#else
	return true;
#endif
}
void ga_udp_server::shutdown_sockets()
{
#if PLATFORM == PLATFORM_WINDOWS
	WSACleanup();
#endif
}

void ga_udp_server::sendSnapshots()
{
	for (int c = 0; c < _clients.size(); c++)
	{
		sendSnapshot(c);
	}
}

int ga_udp_server::receiveData()
{
	ga_address sender;
	std::string buffer;
	int read = _socket->receive(sender, buffer, MAX_BUFFER);
	printf("buffer: %d %s\n", read, buffer);
	// If new client, add to _clients
}

int ga_udp_server::sendSnapshot(int client)
{
	std::string toSend = "Hello client " + client;
	int sending = std::string::length(toSend);
	int sent = _socket->send(_clients.at(client)), toSend, sending);
}

