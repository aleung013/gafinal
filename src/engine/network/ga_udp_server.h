#pragma once
#include "ga_socket.h"
#include "framework/ga_snapshot.h"
#include "framework/ga_frame_params.h"
#include "framework/ga_sim.h"

#define MAX_SNAPSHOTS 32

typedef std::vector<ga_snapshot> snapshot_array;
class ga_udp_server
{
public:
	ga_udp_server(short port, ga_sim* sim);
	~ga_udp_server();
	bool initialize_sockets();
	void shutdown_sockets();
	void update(struct ga_frame_params* params);

private:
	void send_snapshots();
	int send_snapshot(int client);
	char* receive_data(ga_address& sender);
	void handle_command(char * buffer, ga_address sender);


	// Representation
	ga_socket* _socket;
	ga_sim* _sim;
	ga_snapshot _dummy;
	std::vector<snapshot_array> _client_snapshots;
	std::vector<ga_address> _clients;
	std::vector<int> _prev_states;
	int _snapshot_offset;
};