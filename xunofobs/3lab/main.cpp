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


	//Строчка для комманды
	str command = "";
	//Айди очереди сообщений ms(message) q(queue) id(id) сразу ствим его невалидным
	int msqid = -1;

	//Всякое говно для временных всяких штучек
	str tmp;
	int key;
	const char* msg;


	//Говорим что выход это exit
	while (command != "exit") {
		//Хаваем комманду
		std::cin >> command;
		//C = create, V = vendeta
		if (command == "c") {
			//С каким ключем создать очередь
			std::cin >> key;
			msqid = create(static_cast<key_t>(key));
		//D = delete
		} else if (command == "d") {
			if (delete_q(msqid)) {
				//Если получилось то ставим невалидный АйДи
				msqid = -1;
			}
		//R = read
		} else if (command == "r") {
			msg = pull(msqid);
			//Если получили сообщение то печатаем его
			if (msg) {
				std::cout << msg << std::endl;
				//Чистим за собой всякое
				delete msg;
			}
		//W = write
		} else if (command == "w") {
			//Схавали остатки строки
			std::getline(std::cin, tmp);
			//Уберем лишний пробел
			tmp = str(tmp.begin()+1, tmp.end());
			//Токнем все туда
			push(msqid, tmp.c_str());
		} else if (command == "s")
			//Всякие разные там статусы
			status(msqid);
	}


	return 0;
}
