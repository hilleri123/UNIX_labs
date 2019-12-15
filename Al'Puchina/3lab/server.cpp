#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <unistd.h>

#include <fcntl.h>

#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>

//Библа написана на Си поэтому нам нужны СИшные сигнатуры функций
//И как следствие мы объявляем extern "C"
//Сигнатура - типа уневерсальное имя
extern "C" {
#include "functions.h"
}




//аргументы нам не нужны это наследство первых двух прог
int main(int argc, char *argv[]) {
	using str=std::string;	//Хотим псевдоним для строки в 3 буквы вместо дофига


	int qid = -1;

	if ((qid = create(NAME)) < 0) {
		throw std::runtime_error("create queue");
	}


	str tmp;
	while (true) {
		std::getline(std::cin, tmp);
		
		tmp = "("+std::to_string(getpid())+") "+tmp;

		write_q(qid, tmp.c_str());
	}

	delete_q(qid);

	return 0;
}
