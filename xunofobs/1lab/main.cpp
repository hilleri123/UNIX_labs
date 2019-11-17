#include <iostream>
//#include <stdio.h>
#include <grp.h>
//#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <vector>


int main(int argc, char** argv) {	// argv - массив сишных строк полученный из командной строки делением ее по пробелам
	// на командную строку 'check_groups kseniyayaya bla bla bla' -> argv = ['check_groups', 'kseniyayaya', 'bla', 'bla', 'bla'], argc = 5

	using str=std::string;	// Нам впадлу писать много поэтому так
	using vec=std::vector<std::string>;	// Типа список строк потому что!

	
	if (argc == 0) // Если на вход нет ничего кроме имени программы то и отрабатывать нечего
		throw std::invalid_argument("Missing required argument(username)");	// Тут можно было просто завершиться но так он подумает что ты умная мазафака

	// Мне все равно хочу idx вместо i потому что!
	for (int idx = 1; idx < argc; idx++) {	// Обойдем каждое имя


		str name = argv[idx];	// Будем пользоваться плюсовыми строками потому что мы некст генератион а не кто-то там
		vec groups;	// Хочу так потом будет красота только не ревнуй

		// Сишная грязь но без нее никак
		struct group *gr;	// У group есть список char **gr_mem от слова мембер-член Гы гы гы мы его потом подергаем(список)

		// Пусть есть список групп A и указатель на текущую группу curr
		// То setgrent() устанавливает curr на начало A
		// getgrent() возвращает указатель curr и двигает его на некст
		// endgrent() прояви фантазию
		setgrent();
		while ((gr = getgrent()) != NULL) {	// Так можно не удивляйся
			for (int i = 0; gr->gr_mem[i] != NULL; i++) {	// Дергаем список
				str curr_name = gr->gr_mem[i];	// Хочу плюсовые строки
				if (name == curr_name) {
					groups.emplace_back(gr->gr_name);	// Как push_back только он вызовет конструктор std::string от gr->gr_name
				}
			}
		}
		endgrent();


		// Тупа вывод
		std::cout << "Пользователь " << name << " состоит на учете в " << groups.size() << " группах:" << std::endl;
		// Если спрашивает про это а ты не знаешь
		// То берешь распахиваешь свои большие красивые голубые глаза
		// И начинаешь хлопать ими с амплитудой пол метра чтоб его сдуло вместе с сомнениями об авторстве этой работы
		for (auto i = groups.begin(); i < groups.end(); i++)
			std::cout << *i << " ";
		if (groups.size() > 0)	// Строка для красоты не хочешь сопрниц - удоли
			std::cout << std::endl;
	}
	
	// Тупа гуд стайл
	return 0;

	
}


