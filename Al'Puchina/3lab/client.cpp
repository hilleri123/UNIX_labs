#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <unistd.h>

#include <fcntl.h>

#include <sys/time.h>


//Библа написана на Си поэтому нам нужны СИшные сигнатуры функций
//И как следствие мы объявляем extern "C"
//Сигнатура - типа уневерсальное имя
extern "C" {
#include "functions.h"
}




int main(int argc, char *argv[]) {
	using str=std::string;	//Хотим псевдоним для строки в 3 буквы вместо дофига


	mqd_t qid = -1;

	if ((qid = open_q(NAME)) < 0) {
		throw std::runtime_error("open queue");
	}

	while (true) {
		const char* tmp = read_q(qid);
		if (tmp != nullptr) {
			str msg(tmp);
			std::cout << msg << std::endl;
		} 
	}

	delete_q(qid);


	return 0;
}
