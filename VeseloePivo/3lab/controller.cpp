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

	int res;
	str name = "";
	int id = -1;
	std::size_t size = 0;
	bool to_del = false;
	bool to_new = false;
	str save = "";
	str dir = "";
	bool load = false;
	bool show = false;
	while ((res = getopt(argc, argv, "i:s:dnF:D:lS")) > 0) {
		switch (res) {
		case 'i':
			{
				id = atoi(optarg);
				//name = std::string(optarg);
				break;
			}
		case 'S':
			{
				show = true;
				break;
			}
		case 's':
			{
				size = atoi(optarg);
				break;
			}
		case 'd':
			{
				to_del = true;
				break;
			}
		case 'n':
			{
				to_new = true;
				break;
			}
		case 'F':
			{
				save = str(optarg);
				break;
			}
		case 'D':
			{
				dir = str(optarg);
				break;
			}
		case 'l':
			{
				load = true;
				break;
			}
		}
	}

	bool init = false;
	int bd_fd = shm_open(bd_name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
	if (bd_fd < 0) {
		bd_fd = shm_open(bd_name.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		if (bd_fd < 0) {
			throw std::runtime_error("DB shm_open with "+name);
		}
		init = true;
		if (ftruncate(bd_fd, bd_size)) {
			throw std::runtime_error("DB ftruncate");
		}
	}


	bd_file *bd_ptr = static_cast<bd_file*>(mmap(NULL, bd_size, PROT_READ | PROT_WRITE, MAP_SHARED, bd_fd, 0));

	if (bd_ptr == MAP_FAILED) {
		throw std::runtime_error("BD mmap");
	}

	
	//std::cout << "size(" << size << ")" << std::endl;
	for (std::size_t seek = 0; seek < count; seek++) {
		//std::cout << "seek(" << seek << ")" << std::endl;
		//std::cout << "n(" << to_new << ") d(" << to_del << ")" << std::endl;
		bd_file* tmp_ptr = bd_ptr+seek;
		if (init) {
			memcpy(tmp_ptr, new bd_file, sizeof(bd_file));
			tmp_ptr->id = -1;
		}
		if (show && tmp_ptr->id != -1) {
			std::cout << "id(" << tmp_ptr->id << ")" << "name(" << tmp_ptr->name << ")" << "size(" << tmp_ptr->size << ")" << std::endl;
		}
		if (to_new) {
			if (tmp_ptr->id == -1) {
				tmp_ptr->id = id;
				//std::cout << "new succeed" << std::endl;
				strcpy(tmp_ptr->name, str("/aaa"+std::to_string(seek)).c_str());
				tmp_ptr->size = size;
				to_new = false;
				int tmp_fd = shm_open(tmp_ptr->name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
				if (tmp_fd < 0) {
					throw std::runtime_error("shm_open with "+name+" while new");
				}
				if (ftruncate(tmp_fd, tmp_ptr->size)) {
					throw std::runtime_error("ftruncate while new");
				}
				//bd_file *ptr = static_cast<bd_file*>(mmap(NULL, tmp_ptr->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
				//if (ptr == MAP_FAILED) {
					//throw std::runtime_error("mmap while load");
				//}
			}
		}
		if (to_del) {
			if (tmp_ptr->id == id) {
				tmp_ptr->id = -1;
				to_del = false;
			}
		}
	}


	if (save.size() > 0 && dir.size() > 0) {
		if (load) {
			struct stat st;
			int fd_l = open(save.c_str(), O_RDONLY, 0666);
			if (fd_l < 0) {
				throw std::runtime_error("open with "+ save +" while save");
			}
			if (fstat(fd_l, &st)) {
				throw std::runtime_error("fstat while load");
			}
			char* buf = new char[st.st_size];
			if (read(fd_l, buf, st.st_size) < 0) {
				throw std::runtime_error("read while load");
			}
			memcpy(bd_ptr,buf,st.st_size);
			close(fd_l);
			delete buf;

			for (std::size_t seek = 0; seek < count; seek++) {
				bd_file* tmp_ptr = bd_ptr+seek;
				
				//открываем шаред мемори
				if (tmp_ptr->id != -1) {
					std::cout << "id(" << tmp_ptr->id << ") name(" << tmp_ptr->name << ") size(" << tmp_ptr->size << ")" << std::endl;
					int fd = shm_open(tmp_ptr->name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
					if (fd < 0) {
						throw std::runtime_error("shm_open with "+name+" while load");
					}
					if (ftruncate(fd, bd_size)) {
						throw std::runtime_error("ftruncate while load");
					}
					bd_file *ptr = static_cast<bd_file*>(mmap(NULL, tmp_ptr->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
					if (ptr == MAP_FAILED) {
						throw std::runtime_error("mmap while load");
					}
	
					//открываем и читаем из файла
					str file_name = dir+str(tmp_ptr->name);
					fd_l = open(file_name.c_str(), O_RDONLY, 0666);
					if (fd_l < 0) {
						throw std::runtime_error("open with "+ file_name +" while save");
					}
					if (fstat(fd_l, &st)) {
						throw std::runtime_error("fstat while load");
					}
					buf = new char[st.st_size];
					if (read(fd_l, buf, st.st_size) < 0) {
						throw std::runtime_error("read while load");
					}
					memcpy(ptr,buf,st.st_size);
					close(fd_l);
					delete buf;
				}
			}
		} else {
			int fd_s = open(save.c_str(), O_WRONLY, 0666);
			if (fd_s < 0) {
				throw std::runtime_error("open with "+ save +" while save");
			}
			char* buf = new char[bd_size];
			memcpy(buf,bd_ptr,bd_size);
			if (write(fd_s, buf, bd_size) < 0) {
				throw std::runtime_error("write while save");
			}
			close(fd_s);
			delete buf;

			for (std::size_t seek = 0; seek < count; seek++) {
				bd_file* tmp_ptr = bd_ptr+seek;
				
				//открываем шаред мемори
				if (tmp_ptr->id != -1) {
					std::cout << "id(" << tmp_ptr->id << ") name(" << tmp_ptr->name << ") size(" << tmp_ptr->size << ")" << std::endl;
					int fd = shm_open(tmp_ptr->name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
					if (fd < 0) {
						throw std::runtime_error("shm_open with "+name+" while save");
					}
					if (ftruncate(fd, bd_size)) {
						throw std::runtime_error("ftruncate while save");
					}
					bd_file *ptr = static_cast<bd_file*>(mmap(NULL, tmp_ptr->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
					if (ptr == MAP_FAILED) {
						throw std::runtime_error("mmap while save");
					}
	
					//открываем и читаем из файла
					str file_name = dir+str(tmp_ptr->name);
					fd_s = open(file_name.c_str(), O_WRONLY | O_CREAT, 0666);
					if (fd_s < 0) {
						throw std::runtime_error("open with "+ file_name +" while save");
					}
					buf = new char[tmp_ptr->size];
					memcpy(buf,ptr,tmp_ptr->size);
					if (write(fd_s, buf, tmp_ptr->size) < 0) {
						throw std::runtime_error("write while save");
					}
					delete buf;
					close(fd_s);
				}
			}

		}
	}

	return 0;
}
