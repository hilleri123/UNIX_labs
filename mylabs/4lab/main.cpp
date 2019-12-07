#include <iostream>
#include <string>
#include <array>
#include <utility>
#include <fstream>
#include <unistd.h>
#include <cstring>

#include <fcntl.h>

#include <sys/io.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/wait.h>


#define USE_FNCNRL 0


//Хотим 1 аргументом файл
//Последний это какая блокировка
int main(int argc, char *argv[]) {
	using str=std::string;	//Хоти псевдоним для строки в 3 буквы вместо дофига

	//if (argc < 2) {	//Имя этой проги + имя файла = минимум 2
		//throw std::invalid_argument("There's must be file name");	//Типа мы достаточно окультурились чтоб бросать обшибки
	//} else if (argc > 3) {	//Имя этой проги + имя файла + какая блокировка = максисум 3
		//throw std::invalid_argument("Too many arguments");	//Типа мы достаточно окультурились чтоб бросать обшибки
	//}


	const size_t buffer_size = 257;
	std::array<char, buffer_size> buf;

	buf[buffer_size-1] = 0;

	str command = "";
	int seek = 0;
	int shmid = -1;
	int size = 0;
	char* mem = nullptr;
	void* shmaddr = nullptr;
	while (command != "exit") {
		std::cin >> command;	
		if (command == "w" && mem != nullptr) {
			std::cin >> seek;
			std::getline(std::cin, command);
			command = str(++command.begin(), command.end());
			//fputs(command.c_str(), fd);	
			if (size-seek-command.size()+1 >= 0)
				memcpy(mem+seek, command.c_str(), command.size()+1);	
			else
				std::cout << "cannt write '" << command << "' because of size" << std::endl;
		} else if (command == "r" && mem != nullptr) {
			seek = size;
			std::cout << "'";
			while (seek > 0) {
				//std::cout << seek << std::endl;
				if (seek > buf.size()) {
					//std::cout << "memcpy " << memcpy(buf.data(), mem, buf.size()-1) << std::endl;
					memcpy(buf.data(), mem+size-seek, buf.size()-1);
					buf.back() = 0;	
				} else {
					//std::cout << "memcpy " << memcpy(buf.data(), mem, seek) << std::endl;
					memcpy(buf.data(), mem+size-seek, seek);
					buf[seek] = 0;
				}
				std::cout << buf.data();
				seek -= buf.size()-1;
			}
			std::cout << "'" << std::endl;
		} else if (command == "c") {
			std::cin >> seek;
			std::cin >> size;
			shmid = shmget(seek, size, IPC_CREAT | IPC_EXCL | 0666);
			if (shmid >= 0) {
				mem = static_cast<char*>(shmat(shmid, shmaddr, 0));
				if (mem == nullptr)
					std::cout << "shmat faild" << std::endl;
			} else
				std::cout << "shmget faild" << std::endl;
		} else if (command == "d" && mem != nullptr) {
			//if (shmdt(shmaddr) < 0)
			if (shmdt(mem) < 0)
				std::cout << "shmdt faild" << std::endl;
			else {
				size = 0;
				shmid = -1;
				shmaddr = nullptr;
				mem = nullptr;
			}
		} else if (command == "t") {
			std::cin >> seek;
			std::cin >> size;
			shmid = shmget(seek, size, 0666);
			if (shmid >= 0) {
				mem = static_cast<char*>(shmat(shmid, shmaddr, 0));
				if (mem == nullptr)
					std::cout << "shmat faild" << std::endl;
			} else
				std::cout << "shmget faild" << std::endl;
		} else if (command == "exit") {
			break;
		}
	}



	if (mem != nullptr)
		shmdt(mem);
		


	return 0;
}
