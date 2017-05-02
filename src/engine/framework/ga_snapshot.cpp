#include "ga_sim.h"
#include "ga_snapshot.h"
#include "network/ga_socket.h" // For MAX_BUFFER

ga_snapshot::ga_snapshot()
{
	_ack = false;
}

ga_snapshot::ga_snapshot(int num_entities)
{
	_ack = false;
	for (int i = 0; i < num_entities; i++)
	{
		_positions.push_back(ga_vec3f::zero_vector());
	}
}

ga_snapshot::~ga_snapshot()
{
	// Do nothing(?)
}

void ga_snapshot::add_entity(int e,const ga_entity& ent)
{
	_positions[e] = ent.get_transform().get_translation();
}

void ga_snapshot::ack()
{
	_ack = true;
}

char * ga_snapshot::diff(ga_snapshot source, ga_snapshot curr)
{
	char buffer[MAX_BUFFER];
	buffer[0] = '\0';
	// Assume that source and curr have the same number of entities
	for (int i = 0; i < source._positions.size(); i++)
	{
		for (int axis = 0; axis < 3; axis++)
		{
			if (source._positions[i].axes[axis] != curr._positions[i].axes[axis])
			{
				char tmp[MAX_BUFFER];
				sprintf(tmp, "Box %d %d %f ", i, axis, curr._positions[i].axes[axis]);
				strcat(buffer, tmp);
			}
		}
	}
	return buffer;
}
