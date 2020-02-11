#include <iostream>
#include <string>
#include <vector>
#include <map>
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
	int res;

	//Всякое говно для временных всяких штучек
	str tmp;
	key_t key;
	int semid;
	struct sembuf sops[64];
	str name_of_file;


	int nsems;

	std::map<str, std::pair<int, int>> semaphores; 

	//Говорим что выход это quit / q
	while (command != "quit" && command != "q") {
		//Хаваем комманду
		//std::cout << "!!!" << std::endl;
		std::cin >> command;
		//std::cout << "!!!" << std::endl;
		//C = create, V = vendeta
		if (command == "c") {
			//С каким ключем создать очередь
			std::cin >> name_of_file;
			std::cin >> nsems;
			//ftok - file to key
			CALL_FUNC(key, ftok, name_of_file.c_str(), PROJ_ID);
			//semget - возврящает nsems семафор по ключу
			CALL_FUNC(semid, semget, key, nsems, IPC_CREAT | 0666);
			//запомним что получили
			semaphores[name_of_file] = std::make_pair(semid, nsems);
		//O = operate
		} else if (command == "o") {
			std::cin >> name_of_file;
			//Ищем уже открытаю семафору
			auto it = semaphores.find(name_of_file);
			if (it == semaphores.end()) {
				std::cout << name_of_file << " isn't opened" << std::endl;
				continue;
			}
			int ind = 0;
			ushort sem_id;
			short op;
			//Собираем указания
			do {
				std::cin >> command;
				if (command == "\\")
					break;
				switch (ind % 2) {
				case 0:
					{
						sem_id = std::stoi(command);
					}
					break;
				case 1:
					{
						op = std::stoi(command);
						if (ind > 64)
							break;
						sops[ind] = {sem_id, op, IPC_NOWAIT | SEM_UNDO};
						std::cout << "sem id " << sops[ind].sem_num << " op " << sops[ind].sem_op << std::endl;
					}
				}
				ind++;
			} while (command != "\\");
			semid = std::get<0>(std::get<1>(*it));
			//Выполним что насобрали
			//std::cout << "???" << std::endl;
			CALL_FUNC(res, semop, semid, sops, ind);
			//std::cout << "???" << std::endl;
		//R = remove
		} else if (command == "r") {
			std::cin >> name_of_file;
			//Ищем уже открытаю семафору
			auto it = semaphores.find(name_of_file);
			if (it == semaphores.end()) {
				std::cout << name_of_file << " isn't opened" << std::endl;
				continue;
			}
			semid = std::get<0>(std::get<1>(*it));

			CALL_FUNC(res, semctl, 0, IPC_RMID, 0);
			if (res >= 0)
				semaphores.erase(it);
		//S = show
		} else if (command == "s") {
			std::cin >> name_of_file;
			//Ищем уже открытаю семафору
			auto it = semaphores.find(name_of_file);
			if (it == semaphores.end()) {
				std::cout << name_of_file << " isn't opened" << std::endl;
				continue;
			}
			semid = std::get<0>(std::get<1>(*it));

			for (int i = 0; i < std::get<1>(std::get<1>(*it)); i++) {
				int semval = 0;
				CALL_FUNC(semval, semctl, semid, i, GETVAL, 0);
				std::cout << std::get<0>(*it) << " [" << i << "] = " << semval << std::endl;
			}
		//L = list
		} else if (command == "l") {
			for (auto i = semaphores.begin(); i != semaphores.end(); i++) {
				std::pair<int, int> tmp = std::get<1>(*i);
				std::cout << "'" << std::get<0>(*i) << "' : " << std::get<0>(tmp) << " [" << std::get<1>(tmp) << "]" << std::endl;
			}
		} else if (command == "h") {
			std::cout << "c - create semaphore" << std::endl;
			std::cout << "usage : c <file name> <count>" << std::endl;

			std::cout << "o - operate semaphore" << std::endl;
			std::cout << "usage : o <file name> <sem id> <opt> <sem id> <opt> ... <sem id> <opt> \\" << std::endl;

			std::cout << "s - show semaphore value" << std::endl;
			std::cout << "usage : s <file name>" << std::endl;

			std::cout << "r - remove semaphore" << std::endl;
			std::cout << "usage : r <file name>" << std::endl;

			std::cout << "l - list opened semaphores" << std::endl;
		} else if (command != "q" && command != "quit") {
			std::cout << "ignored" << std::endl;
		}

	}


	return 0;
}
