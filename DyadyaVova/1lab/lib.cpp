//#include <iostream>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

//extern C тут чтоб сигнатура функции(имя) была проще (для С это прям ее имя для С++ это еще и символы аргументов и результата)
extern "C" {
	

void run(void) {
	printf("Hello, world!\n");
}


}

extern "C" {
	

void user(void) {
	//Получаем uid - user id
	uid_t uid = geteuid();
	//Получаем всю инфу о юзере по айди
	struct passwd *pw = getpwuid(uid);
	if (pw)
		//Если получилось узнать то пишем имя
		printf("Hello, %s!\n", pw->pw_name);
	else
		//Иначе вот так
		printf("Hello, nameless!\n");
}


}

