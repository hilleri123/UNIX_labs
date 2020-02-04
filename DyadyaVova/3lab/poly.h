#pragma once

#include <cstring>
#include <iostream>
#include <vector>
#include <utility>

std::string del_spaces(std::string);


class Polynom
{
public:
	explicit Polynom(std::string a = "");

	friend Polynom operator+(const Polynom&, const Polynom&);
	friend Polynom operator-(const Polynom&, const Polynom&);
	friend Polynom operator*(const Polynom&, const Polynom&);
	friend Polynom operator*(const Polynom&, double);
	friend Polynom operator*(double , const Polynom&);

	friend std::pair<Polynom, Polynom> operator/(const Polynom&, const Polynom&);
	
	void print() const;
	std::string string() const;

	double at(int i) const { return _vec.at(i); }
	int pow() const { return _vec.size()-1; }
protected:
	void update() {
		while (_vec.size() > 0) {
			if (_vec.back() == 0)
				_vec.pop_back();
			else
				break;
		}
	}
	std::vector<double> _vec;
};


