#include "ga_udp_server.h"
#include <cstdio>
ga_udp_server::ga_udp_server(short port, ga_sim* sim)
{
	initialize_sockets();
	_socket = new ga_socket();
	_socket->open(port);
	_sim = sim;
	_dummy = ga_snapshot(_sim->num_entities());
	_snapshot_offset = 0;
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

void ga_udp_server::handle_command(char* buffer, ga_address sender)
{
	if (strcmp(buffer,"Connect") == 0)
	{
		// Add addr to 
		if (_clients.size() < 4)
		{
			_clients.push_back(sender);
			snapshot_array tmp;
			for (int i = 0; i < MAX_SNAPSHOTS; i++)
			{
				ga_snapshot snap(_sim->num_entities());
				for (int e = 0; e < _sim->num_entities(); e++)
				{
					snap.add_entity(e, *(_sim->get_entity(e)));
				}
				tmp.push_back(snap);
			}
			_client_snapshots.push_back(tmp);
			_prev_states.push_back(-1);
		}
	}
	else if (strstr(buffer, "Key"))
	{
		// Handle keyboard input
		int keyboard_mask = 0;
		char* newbuff = new char[MAX_BUFFER];
		sscanf(buffer, "%s %d", newbuff, &keyboard_mask);
		// Get which box to control
		int player_no;
		for (player_no = 0; player_no < _clients.size(); player_no++)
		{
			if (sender.get_address() == _clients[player_no].get_address() &&
				sender.get_port() == _clients[player_no].get_port())
			{
				break;
			}
		}
		ga_entity* box = _sim->get_entity(player_no);
		ga_vec3f trans = ga_vec3f::zero_vector();
		if (keyboard_mask & k_button_j)
		{
			trans.x = -0.2f;
		}
		else if (keyboard_mask & k_button_l)
		{
			trans.x = 0.2f;
		}
		else if (keyboard_mask & k_button_i)
		{
			trans.z = -0.2f;
		}
		else if (keyboard_mask & k_button_k)
		{
			trans.z = 0.2f;
		}
		box->translate(trans);
	}
	else if (strstr(buffer, "ACK"))
	{
		int snapshot_id = 0;
		// Received an ACK of a snapshot, mark it down in the snapshot history
		sscanf(buffer, "%s %d", buffer, &snapshot_id);
		int player_no;
		for (player_no = 0; player_no < _clients.size(); player_no++)
		{
			if (sender.get_address() == _clients[player_no].get_address() &&
				sender.get_port() == _clients[player_no].get_port())
			{
				break;
			}
		}
		_prev_states[player_no] = snapshot_id;
		_client_snapshots[player_no][snapshot_id].ack();
	}
}
void ga_udp_server::update(ga_frame_params * params)
{
	// Receive commands from clients
	ga_address sender;
	char* buffer = receive_data(sender);
	while (buffer != NULL)
	{
		// Handle command in buffer
		handle_command(buffer, sender);
		// Get next command
		delete buffer; // Free up the buffer from memory
		buffer = receive_data(sender); // Read next packet
	}

	// Master gamestate is ready, time to update snapshots
	for (int c = 0; c < _clients.size(); c++)
	{
		_client_snapshots[c][_snapshot_offset] = ga_snapshot(_sim->num_entities());
		for (int e = 0; e < _sim->num_entities(); e++) 
		{
			_client_snapshots[c][_snapshot_offset].add_entity(e,*(_sim->get_entity(e)));
		}
	}
	// Send snapshots to clients
	send_snapshots();
}

void ga_udp_server::send_snapshots()
{
	for (int c = 0; c < _clients.size(); c++)
	{
		send_snapshot(c);
	}
	_snapshot_offset = (_snapshot_offset + 1) % MAX_SNAPSHOTS;
}

char* ga_udp_server::receive_data(ga_address& sender)
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

int ga_udp_server::send_snapshot(int client)
{
	char to_send[MAX_BUFFER];
	// Write stuff to to_send
	ga_snapshot source, curr;
	if (_prev_states[client] == -1)
	{
		source = _dummy;
	}
	else {
		source = _client_snapshots[client][_prev_states[client]];
	}
	curr = _client_snapshots[client][_snapshot_offset];
	itoa(_snapshot_offset, to_send, 10);
	strcat(to_send, std::string(" ").c_str());
	char* diff = ga_snapshot::diff(source, curr);
	if (strlen(diff) == 0)
	{
		return 0;
	}
	strcat(to_send,ga_snapshot::diff(source, curr));
	int sent = _socket->send(_clients.at(client), to_send, strlen(to_send));
	return sent;
}

