#include "creature.h"
#include "swamp.h"




Creature::Creature(Swamp* s, c_type t) : _type(t), _swamp(s)
{
	if (_swamp == nullptr)
		_alive = false;
	switch (_type) {
	case L:
	{
		_name = "Лужа соды после пикиника";
		_type_to_evolve = c_type::MICRO;
		_w_evolve = 90;
		break;
	}
	case MICRO:
	{
		_name = "рыба-селедка";	//был микроб но тогда его никто не ест
		_type_to_evolve = c_type::REPT;
		_max_age = 6;
		_type_to_kill.push_back(c_type::MICRO);
		_w_breed = 80;
		_w_kill = 40;
		_w_evolve = 30;
		break;
	}
	case REPT:
	{
		_name = "рептилияяя";
		_max_age = 25;
		_type_to_evolve = c_type::TEPL;
		_type_to_kill.push_back(c_type::TEPL);
		_type_to_kill.push_back(c_type::REPT);
		_type_to_kill.push_back(c_type::REPT);
		_type_to_kill.push_back(c_type::MICRO);
		_type_to_kill.push_back(c_type::MICRO);
		_type_to_kill.push_back(c_type::MICRO);
		_w_breed = 70;
		_w_kill = 30;
		_w_evolve = 10;
		break;
	}
	case TEPL:
	{
		_name = "теплокровь";
		_max_age = 60;
		_type_to_evolve = c_type::HUM;
		_type_to_kill.push_back(c_type::TEPL);
		_type_to_kill.push_back(c_type::TEPL);
		_type_to_kill.push_back(c_type::REPT);
		_type_to_kill.push_back(c_type::REPT);
		_type_to_kill.push_back(c_type::REPT);
		_type_to_kill.push_back(c_type::MICRO);
		_type_to_kill.push_back(c_type::MICRO);
		_type_to_kill.push_back(c_type::MICRO);
		_type_to_kill.push_back(c_type::MICRO);
		_type_to_kill.push_back(c_type::HUM);
		_w_breed = 60;
		_w_kill = 40;
		_w_evolve = 10;
		break;
	}
	case HUM:
	{
		_name = "хуман";
		_max_age = 100;
		_type_to_evolve = c_type::HUMPAZ;
		_type_to_kill.push_back(c_type::MICRO);
		_type_to_kill.push_back(c_type::MICRO);
		_type_to_kill.push_back(c_type::REPT);
		_type_to_kill.push_back(c_type::REPT);
		_type_to_kill.push_back(c_type::TEPL);
		_type_to_kill.push_back(c_type::TEPL);
		_type_to_kill.push_back(c_type::HUM);
		_w_breed = 30;
		_w_kill = 50;
		_w_evolve = 15;
		break;
	}
	case HUMPAZ:
	{
		_name = "хуман разумный";
		_max_age = 200;
		_type_to_evolve = c_type::SUPRAZ;
		_type_to_kill.push_back(c_type::HUMPAZ);
		_type_to_kill.push_back(c_type::HUM);
		_type_to_kill.push_back(c_type::REPT);
		_type_to_kill.push_back(c_type::REPT);
		_type_to_kill.push_back(c_type::TEPL);
		_type_to_kill.push_back(c_type::TEPL);
		_type_to_kill.push_back(c_type::MICRO);
		_type_to_kill.push_back(c_type::MICRO);
		_w_breed = 30;
		_w_kill = 60;
		_w_evolve = 20;
		break;
	}
	case SUPRAZ:
	{
		_name = "супер пупур разумный хуман";
		_max_age = 300;
		_type_to_evolve = c_type::ESS;
		_type_to_kill.push_back(c_type::REPT);
		_type_to_kill.push_back(c_type::TEPL);
		_type_to_kill.push_back(c_type::MICRO);
		_w_breed = 10;
		_w_kill = 20;
		_w_evolve = 5;
		break;
	}
	case ESS:
	{
		_name = "когнитивная эссенция разума";
		_max_age = 10000;
		_type_to_evolve = c_type::ESS;
		_type_to_kill.push_back(c_type::MICRO);
		_type_to_kill.push_back(c_type::REPT);
		_type_to_kill.push_back(c_type::TEPL);
		_type_to_kill.push_back(c_type::HUM);
		_type_to_kill.push_back(c_type::HUMPAZ);
		_type_to_kill.push_back(c_type::SUPRAZ);
		_w_kill = 90;
		break;
	}


	}

	
	if (_swamp->add_creature(this))
		std::cout << _name << " is born" << std::endl;

}


bool Creature::breed()
{
	std::lock_guard<std::mutex> lock(_inside);
	if (_alive) {
		if (_type == c_type::L)
			return true;
		Creature* c = new Creature(_swamp, _type);
		_age += 1;
		if (c != nullptr)
			std::cout << _name << " gave birth to " << c->_name << std::endl;
		//return _swamp->add_creature(c);
		return true;
	}
	return false;
}

bool Creature::grow_old()
{
	std::lock_guard<std::mutex> lock(_inside);
	std::cout << "in grow" << std::endl;
	if (_alive) {
		_age += 1;
		switch (_type) {
		case L:
			return true;
		case ESS:
			return true;
		default:
		{
			int p = rand() % 10;
			if (_age + p > _max_age) {
				//die();
				_alive = false;
				std::cout << _name << " died in " << _age << " seconds :(" << std::endl;
				return false;
			}
			return true;
		}
		}
	}
	return false;
}

bool Creature::kill() const
{
	if (_alive) {
		int id = rand() % _type_to_kill.size();
		c_type type_to_kill = _type_to_kill.at(id);
		Creature* c = _swamp->get_creature(type_to_kill);
		if (c != nullptr) {
			if (c->die())
				std::cout << _name << " killed " << c->_name << std::endl;
			else
				std::cout << _name << " tried to kill " << c->_name << " but it's immortal" << std::endl;
			c->unlock();
		}
		std::cout << _name << " wanna kill someone but they're already dead :((((" << std::endl;
	}
	return false;
}

bool Creature::evolve()
{
	std::lock_guard<std::mutex> lock(_inside);
	if (_alive && _type != c_type::ESS) {
		Creature* c = new Creature(_swamp, _type_to_evolve);
		_age += 5;
		if (c != nullptr)
			std::cout << _name << " gave birth to evolved " << c->_name << std::endl;
		//return _swamp->add_creature(c);
		return true;
	}
	return false;
}

bool Creature::die()
{
	std::lock_guard<std::mutex> lock(_inside);
	if (_type == c_type::ESS)
		return false;
	_alive = false;
	//std::cout << _name << " died :(" << std::endl;
	return true;
}



