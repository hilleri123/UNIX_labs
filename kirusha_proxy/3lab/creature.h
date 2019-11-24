#pragma once

#include <cstring>
#include <iostream>
#include <mutex>
#include <vector>
#include <cstdlib>

#include <pthread.h>


enum c_type {L, MICRO, REPT, TEPL, HUM, HUMPAZ, SUPRAZ, ESS};


//#include "swamp.h"


class Swamp;




class Creature
{
public:
	explicit Creature(Swamp*, c_type t = L);

	Creature(const Creature&) = delete;
	Creature(Creature&&) = delete;
	Creature& operator=(const Creature&) = delete;
	Creature& operator=(Creature&&) = delete;

	bool lock() { _mutex.lock(); return true; }
	bool unlock() { _mutex.unlock(); return false; }

	c_type type() const { return _type; }

	bool alive() const { return _alive; }

	bool breed();
	bool grow_old();
	bool kill() const;
	bool evolve();
	bool die();

	int w_kill() const { return _w_kill; }
	int w_breed() const { return _w_breed; }
	int w_evolve() const { return _w_evolve; }

	~Creature() { std::lock_guard<std::mutex> lock(_inside); }

protected:
	c_type _type;
	std::vector<c_type> _type_to_kill;
	c_type _type_to_evolve;

	int _age = 0;
	int _max_age = 0;

	int _w_kill = 5;
	int _w_breed = 50;
	int _w_evolve = 2;


	std::string _name;
	std::mutex _mutex;
	std::mutex _inside;

	Swamp* _swamp;

	bool _alive = true;
};


