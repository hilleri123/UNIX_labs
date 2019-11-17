#include <iostream>
#include <string>
#include <array>
#include <utility>
#include <fstream>
#include <unistd.h>
#include <cstring>

#include <fcntl.h>

#include <sys/io.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/wait.h>


#define USE_FNCNRL 0


//Хотим 1 аргументом файл
//Последний это какая блокировка
int main(int argc, char *argv[]) {
	using str=std::string;	//Хоти псевдоним для строки в 3 буквы вместо дофига

	if (argc < 2) {	//Имя этой проги + имя файла = минимум 2
		throw std::invalid_argument("There's must be file name");	//Типа мы достаточно окультурились чтоб бросать обшибки
	} else if (argc > 3) {	//Имя этой проги + имя файла + какая блокировка = максисум 3
		throw std::invalid_argument("Too many arguments");	//Типа мы достаточно окультурились чтоб бросать обшибки
	}


	const size_t buffer_size = 257;

	std::array<char, buffer_size> buf;	//Тупа буфер размера кратного 8 а именно 257	//Да да 257 не кратно 8 но чтоб это стало строкой последней должен быть 0 так что ща все норм

	str file = argv[1];	//Имя файлоса
	enum lock_type {	//Ну бля читать умеешь разберешься
		DEFAULT,
		WAITING
		//SHARED
	};
	lock_type t = DEFAULT;
	if (argc == 3) {
		if (argv[2] == "-w") {
			t = WAITING;
		//} else if (argv[2] == "-s") {
			//t = SHARED;
		} else {
			t = DEFAULT;	//Нахуй я это написал спросишь меня ты // а я отвечу: Так удоли нахoй раз умный такой //Анивей эта строчка и выше лишние
		}
	}

	FILE* fd = fopen(file.c_str(), "r+");	//Откроем файлочейчек

#if USE_FCNTL
	struct flock fl;	//Настройки для блокировки
	memset(&fl, 0, sizeof(fl));
#endif

	if (fd < 0) {
		throw std::runtime_error("cannt open file :"+file);	//Типа мы достаточно окультурились чтоб бросать обшибки
	} else {
#if USE_FCNTL
		//Если он че спросит там разкомментишь
		fl.l_type = F_RDLCK;	//На чтение
		//fl.l_type = F_WRLCK;	//На запись
		
		//Блокируем весь файл
		fl.l_whence = SEEK_SET;
		fl.l_start = 0;
		fl.l_len = 0;	//0 это специально валуе чтоб получить валуе
#endif

		switch (t) {	//Определим тип блокировки
			case WAITING:
				{
#if !USE_FCNTL
					if (flock(fileno(fd), LOCK_SH) == -1) {
						throw std::runtime_error("Lock Faild");	//Типа мы достаточно окультурились чтоб бросать обшибки
					}
#endif

#if USE_FCNTL
					if (fcntl(fileno(fd), F_SETLKW, &fl) == -1) {
						str err = "";
						if (errno == EACCES || errno == EAGAIN)
							err = ": already locked";
						throw std::runtime_error("Lock Faild"+err);	//Типа мы достаточно окультурились чтоб бросать обшибки
					}
#endif
				}
			//case SHARED:
				//{}
			case DEFAULT:
				{
#if !USE_FCNTL
					if (flock(fileno(fd), LOCK_EX | LOCK_NB) == -1) {
						throw std::runtime_error("Lock Faild");	//Типа мы достаточно окультурились чтоб бросать обшибки
					}
#endif
					
#if USE_FCNTL
					if (fcntl(fileno(fd), F_SETLK, &fl) == -1) {
						str err = "";
						if (errno == EACCES || errno == EAGAIN)
							err = ": already locked";
						throw std::runtime_error("Lock Faild"+err);	//Типа мы достаточно окультурились чтоб бросать обшибки
					}
#endif
				}
		}
	}

	str command = "";
	int seek = 0;
	//Если он спросит почему так красиво мограй глазами и надейся что он гей
	while (command != "exit") {
		std::cin >> command;	//Хотим команжу
		if (command == "w") {	//Теперь мы писатель
			std::cin >> seek;	//Хотим позицию с которой писать
			fseek(fd, seek, SEEK_SET);	//Ставим корретку куда нам надо
			std::getline(std::cin, command);	//Берем хуйню	//И похуй что берем в command	//Мы дикие нам вообще поебать
			command = str(++command.begin(), command.end());	//Изравляемся от разделителя в начале
			fputs(command.c_str(), fd);	//Пишем хуйню
			//std::cout << "'" << command << "'" << std::endl;
		} else if (command == "r") {
			std::cin >> seek;	//Хотим позицию с которой писать
			fseek(fd, seek, SEEK_SET);	//Ставим корретку куда нам надо
			std::cin >> seek;	//Теперь мы забираем сколько прочитать
			while (seek > 0) {
				if (seek > buf.size()) {
					fread(buf.data(), buf.size()-1, sizeof(char), fd);	//Читаем сколько могем
					buf.back() = 0;	//СИшная строка кончается 0 так шо ставим нулину
				} else {
					fread(buf.data(), seek, sizeof(char), fd);	//Читаем сколько положено
					buf[seek] = 0;	//СИшная строка кончается 0 так шо ставим нулину	
				}
				//std::cout << "'" << buf.data() << "'" << std::endl;
				std::cout << buf.data() << std::endl;
				seek -= buf.size()-1;
			}
		} else if (command == "unlock") {	//Разлочим
#if !USE_FCNTL
			if (flock(fileno(fd), LOCK_UN) == -1)
				throw std::runtime_error("Unlock Faild");	//Типа мы достаточно окультурились чтоб бросать обшибки
#endif
			
#if USE_FCNTL
			fl.l_type = F_UNLCK;
			if (fcntl(fileno(fd), F_SETLKW, &fl) == -1)
				throw std::runtime_error("Unlock Faild");	//Типа мы достаточно окультурились чтоб бросать обшибки
#endif
		}
	}




	fclose(fd);
		


	return 0;
}
