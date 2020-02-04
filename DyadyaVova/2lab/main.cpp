#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

//Переменная состояния - перехватывать ctrl+С или нет
static bool toggle = true;

//Функция которую будем вызывать на приход сиганала
void my_handler(int s){
	//ctrl+C это 2 и проверяем переменную
	if (s == 2 && toggle) {
		//надо сказать что мы заигнорили
		printf("IGNORED!!!!\n");
		//На стакеоверфлоу так было но мне кажется что это не нужно. если что раскомментишь
		//signal(s, my_handler);
	} else {
		//Выйдем если перехватили сигнал
		printf("Caught signal %d\n",s);
		exit(1); 
	}
}


int main(int argc,char** argv)
{
	//Связываем ctrl+C с нашей функцией выше
	signal(2, my_handler);

	//С с++ строке будем держать комманду чтоб удобнее было
	std::string command = "";
	//Пока не exit будем работать
	while (command != "exit") {
		//считываем комманду
		std::cin >> command;
		//Если хелп то напишем что можно делать прост чтоб не забыть(хелп всегда помнишь а вот остальное)
		if (command == "help" || command == "h") {
			std::cout << "switch or s to cought ctrl+C" << std::endl;
		} else if (command == "switch" || command == "s") {
			//Переключаем и пишем что происходит с нашим ловцом
			if (toggle)
				std::cout << "coughter sleeping" << std::endl;
			else
				std::cout << "coughter running" << std::endl;
			//Вот само переключение
			// !true = flase
			// !false = true
			toggle = !toggle;
		}
	}

	return 0;    
}
