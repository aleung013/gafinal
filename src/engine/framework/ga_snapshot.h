#pragma once
#include <vector>
#include "../entity/ga_entity.h"

class ga_snapshot
{
public:
	ga_snapshot();
	ga_snapshot(int num_entities);
	~ga_snapshot();
	
	void add_entity(int e, const ga_entity& ent);
	void ack();
	static char* diff(ga_snapshot source, ga_snapshot curr);
private:
	std::vector<ga_vec3f> _positions;
	bool _ack;
};