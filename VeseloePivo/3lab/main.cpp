#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <unistd.h>

#include <fcntl.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "conf.h"


int main(int argc, char *argv[]) {
	using str=std::string;	//Хотим псевдоним для строки в 3 буквы вместо дофига

	//int res;
	str name = "";
	std::size_t size = 0;
#if 0
	while ((res = getopt(argc, argv, "n:s:")) > 0) {
		switch (res) {
		case 'n':
			{
				name = std::string(optarg);
				break;
			}
		case 's':
			{
				size = atoi(optarg);
				break;
			}
		}
	}
#endif

	int bd_fd = shm_open(bd_name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
	if (bd_fd < 0) {
		throw std::runtime_error("DB shm_open with "+name);
	}

	//if (ftruncate(bd_fd, bd_size)) {
		//throw std::runtime_error("DB ftruncate");
	//}

	bd_file *bd_ptr = static_cast<bd_file*>(mmap(NULL, bd_size, PROT_READ | PROT_WRITE, MAP_SHARED, bd_fd, 0));

	if (bd_ptr == MAP_FAILED) {
		throw std::runtime_error("BD mmap");
	}
	

	str command = "";
	char *ptr = nullptr;

	while (command != "exit") {
		std::cin >> command;
		if (command == "o") {
			int id = 0;
			std::cin >> id;
			
			for (std::size_t seek = 0; seek < count; seek++) {
				bd_file *tmp_ptr = bd_ptr+seek;
				if (tmp_ptr->id == id) {
					name = str(tmp_ptr->name);
					size = tmp_ptr->size;
				}
			}

			int fd = shm_open(name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
			if (fd < 0) {
				fd = shm_open(name.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
				if (fd < 0) {
					throw std::runtime_error("shm_open with "+name);
				}
				if (ftruncate(fd, size)) {
					throw std::runtime_error("ftruncate");
				}
			}
		
		
			ptr = static_cast<char*>(mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
		
			if (ptr == MAP_FAILED) {
				throw std::runtime_error("mmap");
			}
			std::cout << "opened with name(" << name << ") and size(" << size << ")" << std::endl;
	

		} if (command == "w") {
			if (ptr != nullptr) {
				str to_write = "";
				std::getline(std::cin, to_write);
				to_write = str(++to_write.begin(), to_write.end());
				if (to_write.size() > 0) {
					strcpy(ptr, to_write.c_str());
				}
			}
		} else if (command == "r") {
			if (ptr != nullptr) {
				std::cout << "'" << ptr << "'" << std::endl;
			}	
		}
	}


	return 0;
}
