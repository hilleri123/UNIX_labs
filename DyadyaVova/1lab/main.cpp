#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include <dlfcn.h>


//Выглядит примерно так 
// ./Run 
// a ./lib.so
// f ./lib.so run user
// c ./lib.so
// Последнее не обяз.


int main(int argc, char *argv[]) {
	//Чтоб не писать std::vec... будем писать svec
	using svec=std::vector<std::string>;
	//Чтоб не писать std::vec... будем писать vec
	//А хранить тут будем указатель на уже открытую библиотеку + ее имя
	using vec=std::vector<std::pair<void*, std::string>>;

	//Выходить или нет
	bool exit = false;
	//Буфер для комманд
	std::string buf;
	//Библы
	vec libs;

	while (!exit) {
		char buff[256];
		
		//Получаем из ввода в буфер
		std::cin.getline(buff, sizeof(buff));
		//Засовываем в основной буфер
		buf = std::string(buff);

		//Чтоб разбить по поробелам
		svec args;
		auto idx = buf.begin();

		//Разбиваем на пробелы
		for (auto i = buf.begin(); i < buf.end(); i++) {
			if (*i == ' ') {
				args.emplace_back(idx, i);
				idx = i + 1;
			}
		}
		args.emplace_back(idx, buf.end());
		//Если комманда a(append)
		if (*args.begin() == "a") {
			//По словам после 1ого
			for (auto i = args.begin()+1; i < args.end(); i++) {
				//Покажем что мы прочитали
				std::cout << *i << std::endl;
				//Хз зачем тут еще раз эта проверка
				if (*(args.begin()) == "a") {
					//Пытаемся открыть библу по имени
					auto lib = dlopen(i->c_str(), RTLD_LAZY);
					//Броим ошибку если все плохо
					if (!lib) {
						std::cout << "cannot open lib " << *i << std::endl;
						std::cout << "dlerror :" << dlerror() << std::endl;
					} else 
						//Добавим открытую либу в на вектор либ
						libs.push_back(std::make_pair(lib, *i));
				}
				dlerror();
			}
		//l - list покажем все открытые либы
		} else if (*args.begin() == "l") {
			for (auto i = libs.begin(); i < libs.end(); i++) {
				//В паре 0 - это указатель а 1 это имя поэтому тут std::get<1>()
				std::cout << "'" << std::get<1>(*i) << "' ";
			}
			std::cout << std::endl;
		//f - function выполняем функцию (f имя_либы имя_функции0 имя_функции1 имя_функции2 ...)
		//Тоесть можно по несколько функций из либы запускать
		} else if (*args.begin() == "f") {
			void* lib = nullptr;
			std::string name;
			for (auto i = args.begin()+1; i < args.end(); i++) {
				//Если у нас уже стоит либа то пытаемся открыть функцию
				if (lib) {
		       			auto fun = (void (*)(void))dlsym(lib, i->c_str());
					if (!fun) {
  						std::cout << "cannot load function '" << *i << "' from " << name << std::endl;
						std::cout << "dlerror :" << dlerror() << std::endl;
					} else
  						fun();
					dlerror();
				//Если нет либы то найдем ее
				} else {
					//Считаем что мы получили имя либы
					name = *i;
					//Проходим по либам ищем соответствие
					for (auto j = libs.begin(); j < libs.end(); j++) {
						if (std::get<1>(*j) == *i) {
							//Все что надо нашли и запомнили либу
							lib = std::get<0>(*j);
							break;
						}
					}
					if (!lib) {
						std::cout << *i << " isn't opened" << std::endl;
					}
				}
			}
		//c - close закроем либу по имени
		} else if (*args.begin() == "c") {
			for (auto i = args.begin()+1; i < args.end(); i++) {
				auto del = libs.end();
				void* lib = nullptr;
				//сначала ее найдем
				for (auto j = libs.begin(); j < libs.end(); j++) {
					if (std::get<1>(*j) == *i) {
						lib = std::get<0>(*j);
						break;
					}
				}
				if (!lib) {
					std::cout << *i << " isn't opened" << std::endl;
				} else {
					//Если нашли то закроем ее
					dlclose(lib);
					libs.erase(del);
				}
			}
			//if (fun_idx > lib_idx) {
				////std::cout << "c str " << fun_idx->c_str() << " " << lib_idx->c_str() << std::endl;
				////void (*fun)(void);
		       		//auto fun = (void (*)(void))dlsym(lib, fun_idx->c_str());
				//if (!fun) {
  					//std::cout << "cannot load function '" << *fun_idx << "' from " << *lib_idx << std::endl;
					//std::cout << "dlerror :" << dlerror() << std::endl;
					//} else
  				//fun();
				//dlerror();
				//fun_idx = vargv.begin();
			//}
			//dlclose(lib);
		} else if (*args.begin() == "exit") {
			//Если выход то выходим(почти очевидно)
			exit = true;
			//std::cout << "exit " << exit << std::endl;
		}
	}
	//Закроем все либы перед выходом
	for (auto j = libs.begin(); j < libs.end(); j++) {
		dlclose(std::get<0>(*j));
	}
	return 0;
}


