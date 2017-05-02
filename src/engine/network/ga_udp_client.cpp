#include "ga_udp_client.h"
#include <string>
#include <sstream>
ga_udp_client::ga_udp_client(short port, ga_address server, ga_sim* sim)
{
	initialize_sockets();
	_socket = new ga_socket();
	_socket->open(port);
	_server = server;
	_sim = sim;
	char* buffer = "Connect";
	_socket->send(server, buffer, strlen(buffer));
}

ga_udp_client::~ga_udp_client()
{
	delete _socket;
	shutdown_sockets();
}

bool ga_udp_client::initialize_sockets()
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
void ga_udp_client::shutdown_sockets()
{
#if PLATFORM == PLATFORM_WINDOWS
	WSACleanup();
#endif
}

void ga_udp_client::update(struct ga_frame_params* params) {
	// Send and commands being pressed
	unsigned int keyboard = params->_button_mask;
	char* buffer = new char[MAX_BUFFER];
	if (keyboard != 0)
	{
		sprintf(buffer, "Key %d", keyboard);
	}
	send(buffer);
	delete buffer;

	// Receive new snapshots from server
	ga_address sender;
	buffer = receive(sender);
	if (buffer != NULL)
	{
		int snapshot_id;
		std::istringstream is(buffer);
		std::string token;
		int i = 0, axis = 0;
		float pos = 0.0f;
		is >> snapshot_id;
		while (is)
		{
			is >> token;
			if (token == std::string("Box"))
			{
				is >> i >> axis >> pos;
				ga_entity* ent = _sim->get_entity(i);
				ga_vec3f trans = ga_vec3f::zero_vector();
				trans.axes[axis] = pos - ent->get_transform().get_translation().axes[axis];
				ent->translate(trans);
			}
		}
		// Parse buffer and update new positions
		// Send ACK to server
		sprintf(buffer, "ACK %d", snapshot_id);
		send(buffer);
	}
}

int ga_udp_client::send(const void * data)
{
	return _socket->send(_server, data, strlen((char*)data));
}

char* ga_udp_client::receive(ga_address& sender)
{
	char* buffer = new char[MAX_BUFFER];
	int read = _socket->receive(sender, buffer, MAX_BUFFER);
	if (read == 0)
	{
		return NULL;
	}
	buffer[read] = '\0';
	return buffer;
}
