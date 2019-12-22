#include <cstring>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <rpc/rpc.h>

#include <array>
#include <utility>
#include <fstream>
#include <unistd.h>

#include <fcntl.h>

#include <sys/io.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/wait.h>


#define USE_FNCNRL 0



#include "del.h"


char **
str_to_c(const std::string& s)
{
	char ** res = new char*;
	*res = new char[s.size()+1];
	memcpy(*res, s.c_str(), s.size()+1);

	return res;
}


char **
del_1(pol * p, CLIENT * clnt)
{
	char* file = p->file;
	int seek = p->seek;
	int size = p->size;
	char* c = p->com;
	char* str = p->s;

	std::string command(c);

	const size_t buffer_size = 257;
	std::array<char, buffer_size> buf;	//Тупа буфер размера кратного 8 а именно 257	//Да да 257 не кратно 8 но чтоб это стало строкой последней должен быть 0 так что ща все норм

	FILE* fd = fopen(file, "r+");	//Откроем файлочейчек

	std::string res;

	if (fd <= 0) {
		//throw std::runtime_error("cannt open file :"+file);	//Типа мы достаточно окультурились чтоб бросать обшибки
		fd = fopen(file, "w+");	//Откроем файлочейчек
		if (fd <= 0) {
			res = "-1";
			return str_to_c(res);
		}
	}

	//std::cout << command << file << seek << std::endl;

	if (command == "w") {	//Теперь мы писатель
		fseek(fd, seek, SEEK_SET);	//Ставим корретку куда нам надо
		fputs(str, fd);	//Пишем хуйню
	} else if (command == "r") {
		fseek(fd, seek, SEEK_SET);	//Ставим корретку куда нам надо
		//std::cout << size << std::endl;
		while (size > 0) {
			if (size > buf.size()) {
				fread(buf.data(), buf.size()-1, sizeof(char), fd);	//Читаем сколько могем
				buf.back() = 0;	//СИшная строка кончается 0 так шо ставим нулину
			} else {
				fread(buf.data(), size, sizeof(char), fd);	//Читаем сколько положено
				buf[size] = 0;	//СИшная строка кончается 0 так шо ставим нулину	
			}
			//std::cout << "'" << buf.data() << "'";
			res += std::string(buf.data());
			size -= buf.size()-1;
		}
		//std::cout << std::endl;
	} else {
		res = "-2";
		return str_to_c(res);
	}




	fclose(fd);
		
	//std::cout << "close" << std::endl;

	if (res.size() == 0)
		res = "0";

	return str_to_c(res);
}

char **
//del_1_svc(char** t, struct svc_req *svc)
del_1_svc(pol * t, struct svc_req *svc)
{
	//printf("%s\n", t->first);
	CLIENT * clnt;
	return del_1(t, clnt);
}









