#include <iostream>
#include <cstring>
#include <mutex>
#include <vector>
#include <cstdlib>

#include <pthread.h>
#include <unistd.h>

#include "swamp.h"




bool Swamp::add_creature(Creature* c)
{
	std::lock_guard<std::mutex> lock(_mutex);
	//std::cout << "add" << std::endl;
	if (c != nullptr) {
		c->lock();
		if (c->alive()) {
			pthread_t tid;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			if (pthread_create(&tid, &attr, live, static_cast<void*>(c)) == 0) {
				//std::cout << "push" << std::endl;
				_all.push_back(c);
				//std::cout << "pushed" << std::endl;
				c->unlock();
				//std::cout << "unlock" << std::endl;
				return true;
			}
		}
		c->unlock();
	}
	return false;
}

Creature* Swamp::get_creature(c_type type)
{
	std::lock_guard<std::mutex> lock(_mutex);
	for (auto i = _all.begin(); i < _all.end(); i++) {
		if (*i != nullptr) {
			Creature * c = *i;
			if (c->lock())
				if (c->type() == type && c->alive())
					return c;
		}
	}
	return nullptr;
}

Swamp::~Swamp()
{
	std::lock_guard<std::mutex> lock(_mutex);
	for (auto i = _tid.begin(); i < _tid.end(); i++) {
		if (pthread_cancel(*i) != 0)
			std::cout << "pthread_cancel failed" << std::endl;
		pthread_join(*i, NULL);
	}
	for (auto i = _all.begin(); i < _all.end(); i++) {
		(*i)->~Creature();
	}
}



void* live(void* v)
{
	Creature* c = static_cast<Creature*>(v);
	std::cout << "live" << std::endl;

	while (true) {
		c->lock();
		if (!c->alive()) {
			c->unlock();
			//std::cout << "unlive" << std::endl;
			pthread_exit(0);
		}
		std::cout << "unlock state" << std::endl;
		c->grow_old();
		std::cout << "g ununlock state" << std::endl;
		int s = rand() % 100;
		if (s < c->w_kill())
			c->kill();
		std::cout << "k ununlock state" << std::endl;
		if (s < c->w_breed())
			c->breed();
		std::cout << "b ununlock state" << std::endl;
		if (s < c->w_evolve())
			c->evolve();
		std::cout << "e ununlock state" << std::endl;
		c->unlock();
		sleep(1);
	}
}
