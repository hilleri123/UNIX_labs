
#include "poly.h"

std::string del_spaces(std::string s)
{
	for (int i = 0; i < s.length(); i++)
		if (s[i] == ' ')
			s.erase(i--, 1);
	return s;
}


Polynom::Polynom(std::string s)
{
	int pow = 0;
	while (s.length() > 0) {
		
		std::string tem = "x^"+std::to_string(pow);
		int pos = s.find("x^"+std::to_string(pow));
		//std::cout << "s:" << s << " t:" << tem << " pos:" << pos << std::endl;
		if (pos < 0)
			_vec.push_back(0);
		else {
			s.erase(pos, tem.length());
			//std::cout << "new s:" << s << std::endl;
			int i = pos - 1;
			for (; i > 0; i--) {
				if (s[i] == '-' || s[i] == '+' || !std::isdigit(s[i]))
					break;
			}
			std::string tmp;
			//std::cout << "pos:" << pos << " i:" << i << std::endl;
			if (pos-i > 0 && i >= 0) {
				tmp = s.substr(i, pos-i);
				//std::cout << "tmp:" << tmp << std::endl;
				s.erase(i, pos-i);
				//std::cout << "new s:" << s << std::endl;
			}
			if (tmp.length() < 1)
				tmp += "1";
			else if (tmp.length() == 1)
				if (!std::isdigit(tmp[0]))
					tmp += "1";
			//std::cout << tmp << std::endl;
			_vec.push_back(std::stod(tmp));
		}
	

		pow++;
		s = del_spaces(s);
	}
}

void Polynom::print() const
{
	std::cout << string() << std::endl;
}

std::string Polynom::string() const
{
	std::string res;
	for (int pow = 0; pow <= this->pow(); pow++) {
		if (at(pow) != 0)
			if (pow == this->pow() || at(pow) < 0)
				res = std::to_string(at(pow)) + "x^" + std::to_string(pow) + res;
			else 
				res = "+" + std::to_string(at(pow)) + "x^" + std::to_string(pow) + res;
	}
	return res;
}

Polynom operator+(const Polynom& f, const Polynom& s)
{
	Polynom res = s;
	for (int pow = 0; pow <= f.pow(); pow++) {
		if (res.pow() < pow)
			res._vec.push_back(f.at(pow));
		else 
			res._vec.at(pow) = res.at(pow) + f.at(pow);
	}
	res.update();
	return res; 
}
Polynom operator*(const Polynom& f, double s)
{
	Polynom res = f;
	for (int pow = 0; pow <= res.pow(); pow++) {
		res._vec.at(pow) = s * res.at(pow);
	}
	return res;
}

Polynom operator*(double s, const Polynom& f) { return f*s; }

Polynom operator-(const Polynom& f, const Polynom& s)
{
	Polynom res = s * -1.;
	//for (int pow = 0; pow <= res.pow(); pow++) {
		//res._vec.at(pow) = -res.at(pow);
	//}
	return res + f;
}

Polynom operator*(const Polynom& f, const Polynom& s)
{
	Polynom res;
	//f.print();
	//s.print();
	for (int pow = 0; pow <= s.pow(); pow++) {
		Polynom tmp = f * s.at(pow);
		tmp._vec.insert(tmp._vec.begin(), pow, 0);
		//tmp.print();
		res = res + tmp;
	}
	return res;
}

std::pair<Polynom, Polynom> operator/(const Polynom& f, const Polynom& s)
{
	Polynom res;
	Polynom dec(f);
	for (int pow = dec.pow(); pow >= 0 && dec.pow() >= s.pow(); pow--) {
		double k = dec.at(dec.pow()) / s.at(s.pow());
		res._vec.insert(res._vec.begin(), k);
		Polynom tmp = s * k;
		tmp._vec.insert(tmp._vec.begin(), dec.pow()-s.pow(), 0);
		dec = dec - tmp;


	}
	return std::make_pair(res, dec);
}

