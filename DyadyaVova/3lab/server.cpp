#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <cstring>


#include "poly.h"

int main(int argc, char *argv[])
{
	//ld - listen discriptor
	//cd - connection descriptor
	int ld = 0, cd = 0;
	struct sockaddr_in serv_addr; 

	const int N = 256;
	char buf[N+1];

	//Делаем сокет
	ld = socket(AF_INET, SOCK_STREAM, 0);

	//Заполняем структуру на локаль (если я правильно понял)
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000); 

	//Биндимся
	bind(ld, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

	//Начинаем слушать
	listen(ld, 10); 

	//Она не нужна но нам нужно что-то послать на то что мы приняли первый полином и можно слать второй 
	//Пусть мы пошлем что-то внятное +-
	std::string hello = "send moreee";

	//На сервере не нужно чекать ошибки на все равно что ты нам сделаешь мы на сервере
	while(1)
	{
		//Принимаем соединение
		cd = accept(ld, (struct sockaddr*)nullptr, NULL); 

		//Читаем первый полином
		read(cd, buf, N);
		//Собираем его из строки (класс Polynom сделает все сам)
		Polynom a = Polynom(std::string(buf));
		//Очистим буфер чтоб если следующее сообщение меньше этого мы не оставили свой мусор
		memset(&buf, 0, N+1);

		//Пишем что прочитали первый полином
		write(cd, hello.c_str(), strlen(hello.c_str()));

		//Читаем второй
		read(cd, buf, N);
		Polynom b = Polynom(std::string(buf));
		memset(&buf, 0, N+1);

		//Умножаем 
		Polynom c = a * b;
		//Пишем ответ
		write(cd, c.string().c_str(), strlen(c.string().c_str()));

		//Закрываем соединение 
		close(cd);
		//спис 1 сек
		sleep(1);
	}
}
