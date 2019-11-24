#pragma once

#include <iostream>
#include <cstring>
#include <mutex>
#include <vector>

#include <pthread.h>

#include "creature.h"

//class Creature;

//enum c_type;

class Swamp
{
public:
	explicit Swamp() {}

	bool add_creature(Creature* c);

	Creature* get_creature(c_type type);

	~Swamp();

protected:
	
	std::vector<pthread_t> _tid;

	std::vector<Creature*> _all;
	std::mutex _mutex;
};


void* live(void *);

