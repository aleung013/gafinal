#pragma once
#include "ga_socket.h"
#include "framework/ga_frame_params.h"
#include "framework/ga_sim.h"
#include "entity/ga_entity.h"
class ga_udp_client
{
public:
	ga_udp_client(short port, ga_address server, ga_sim* sim);
	~ga_udp_client();
	bool initialize_sockets();
	void shutdown_sockets();
	void update(struct ga_frame_params* params);
private:
	int send(const void* data);
	char* receive(ga_address& sender);
	// Representation
	ga_socket* _socket;
	ga_address _server;
	ga_sim* _sim;
};