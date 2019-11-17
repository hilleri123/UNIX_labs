#include <cstdio>
#include <iostream>
#include <string>
#include <utility>
#include <cmath>

#include <unistd.h>
#include <stdlib.h>



int main(int argc, char *argv[]) {

#if 0
	if (argc != 4)
		throw std::invalid_argument("There's must be 3 argumets but got "+std::to_string(argc-1));

	//ax^2+2bx+c=0
	double a = std::stod(argv[1]);
	double b = std::stod(argv[2]);
	double c = std::stod(argv[3]);
#endif
	double a = 0;
	double b = 0;
	double c = 0;
	//std::cout << "child" << std::endl;
	std::cin >> a;
	//std::cout << "set a" << std::endl;
	std::cin >> b;
	//std::cout << "set b" << std::endl;
	std::cin >> c;
	//std::cout << "set c" << std::endl;

	//int fd[] = {0, std::stoi(argv[4])};

	double D = b*b-4*a*c;

	std::string str;
	if (D < 0) {
		str = "D < 0 решений нет";
		std::cout << str << std::endl;
		//wrire(fd[1], str.c_str(), str.length());
		return 0;
	} else {
		double x1 = (-b+sqrt(D))/2;
		if (D == 0) {
			str = "D == 0 одно решение " + std::to_string(x1);
			std::cout << str << std::endl;
			//wrire(fd[1], str, str.length());
			return 0;
		}
		double x2 = (-b-sqrt(D))/2;
		str = "D > 0 два решения " + std::to_string(x1) + " " + std::to_string(x2);
		std::cout << str << std::endl;
		//wrire(fd[1], str, str.length());
		return 0;

	}
	return 0;
}
