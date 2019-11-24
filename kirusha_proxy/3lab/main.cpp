#include <iostream>
#include <cstring>
#include <mutex>
#include <vector>

#include <unistd.h>

#include "creature.h"

#include "swamp.h"



int main()
{
	Swamp swamp;
	Creature* first_one = new Creature(&swamp);

	sleep(100000);
};

