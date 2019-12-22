#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <utility>
#include <fstream>
#include <unistd.h>

#include <fcntl.h>

#include <pthread.h>
#include <sys/time.h>

#include <errno.h>



void* routine(void * ptr)
{
	sleep(10);
	if (ptr == nullptr)
		std::cout << "bla bla bla" << std::endl;
	else 
		std::cout << "'" << static_cast<char*>(ptr) << "'" << std::endl;
	pthread_exit(nullptr);
}


//аргументы нам не нужны это наследство первых двух прог
int main(int argc, char *argv[]) {
	using str=std::string;	//Хотим псевдоним для строки в 3 буквы вместо дофига


	//Строчка для комманды
	str command = "";
	str tmp = "";

	std::vector<pthread_t*> threads;

	pthread_attr_t attr;

	pthread_attr_init(&attr);


	int status;

	//Говорим что выход это exit
	while (command != "exit") {
		//Хаваем комманду
		std::cin >> command;
		//C = create, V = vendeta
		if (command == "set" || command == "get") {
			bool r = command == "get";
			std::cin >> command;
			if (command == "size") {
				if (!r)
					std::cin >> command;
				std::size_t tmp_s;
				if (r) {
					status = pthread_attr_getstacksize(&attr, &tmp_s);
					std::cout << "size = "<< tmp_s << std::endl;
				} else {
					status = pthread_attr_setstacksize(&attr, std::stoi(command));
				}
				std::cout << "status:" << status << "(" << strerror(status) << ")" << std::endl;
			} else if (command == "scope") {
				std::cout << "PTHREAD_SCOPE_SYSTEM(" << PTHREAD_SCOPE_SYSTEM << ") PTHREAD_SCOPE_PROCESS(" << PTHREAD_SCOPE_PROCESS << ")" << std::endl;
				if (!r)
					std::cin >> command;
				int tmp_s;
				if (r) {
					status = pthread_attr_getscope(&attr, &tmp_s);
					std::cout << "scope = "<< tmp_s << std::endl;
				} else {
					status = pthread_attr_setscope(&attr, std::stoi(command));
				}
				std::cout << "status:" << status << "(" << strerror(status) << ")" << std::endl;
			} else if (command == "policy") {
				std::cout << "SCHED_FIFO(" << SCHED_FIFO << ") SCHED_RR(" << SCHED_RR << ") SCHED_OTHER(" << SCHED_OTHER << ")" << std::endl;
				if (!r)
					std::cin >> command;
				int tmp_s;
				if (r) {
					status = pthread_attr_getschedpolicy(&attr, &tmp_s);
					std::cout << "policy = "<< tmp_s << std::endl;
				} else {
					status = pthread_attr_setschedpolicy(&attr, std::stoi(command));
				}
				std::cout << "status:" << status << std::endl;
				std::cout << "status:" << status << "(" << strerror(status) << ")" << std::endl;
			} else if (command == "detach") {
				std::cout << "PTHREAD_CREATE_DETACHED(" << PTHREAD_CREATE_DETACHED << ") PTHREAD_CREATE_JOINABLE(" << PTHREAD_CREATE_JOINABLE << ")" << std::endl;
				if (!r)
					std::cin >> command;
				int tmp_s;
				if (r) {
					status = pthread_attr_getdetachstate(&attr, &tmp_s);
					std::cout << "state = "<< tmp_s << std::endl;
				} else {
					status = pthread_attr_setdetachstate(&attr, std::stoi(command));
				}
				std::cout << "status:" << status << "(" << strerror(status) << ")" << std::endl;
			}
		} else if (command == "run") {
			//std::cin >> command;
			std::getline(std::cin, tmp);
			//Уберем лишний пробел
			tmp = str(tmp.begin()+1, tmp.end());
			//Токнем все туда
			char* str_tmp = new char[tmp.size()+1];
			str_tmp[tmp.size()] = 0;
			strcpy(str_tmp, tmp.c_str());

			threads.push_back(new pthread_t);
			pthread_create(threads.back(), &attr, routine, static_cast<void*>(str_tmp));
		}
	}

	for (auto i = threads.begin(); i < threads.end(); i++) {
		pthread_join(**i, nullptr);
	}

	return 0;
}
