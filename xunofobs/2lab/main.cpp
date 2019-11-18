#include <iostream>
#include <string>
#include <array>
#include <utility>
#include <fstream>
#include <unistd.h>

#include <fcntl.h>

#include <sys/io.h>
#include <sys/types.h>
#include <sys/wait.h>


//Хотим 1 аргументом прогу последним файл
//Все что между аргументы проги
int main(int argc, char *argv[]) {
	using str=std::string;	//Хоти псевдоним для строки в 3 буквы вместо дофига

	if (argc < 3) {	//Имя этой проги + имя проги что запустим + файл куда = минимум 3
		throw std::invalid_argument("There's must be program name and output file");	//Типа мы достаточно окультурились чтоб бросать обшибки
		return 1;
	}


	const size_t buffer_size = 256;

	std::array<char, buffer_size> buf;	//Тупа буфер размера кратного 8 а именно 256

	str program = argv[1];	//Имя программулины

	str args = "";	//Аргументы
	char** args_c = static_cast<char**>(malloc(sizeof(char**)*(argc-2)));	//Аргументы дочки маминой подруги
	//Если он спросит почему так мограй глазами и надейся что он не гей
	for (int i = 1; i < argc-1; i++) {
		args += " " + str(argv[i]);	//Разделяем все через пробел почему так потому что потом если сложить то будет красота
		args_c[i-1] = argv[i];
	}

	str file = argv[argc-1];	//Как обещали последний это файл



	int fp = open(file.c_str(), O_CREAT|O_RDWR, 0666);	//Откроем файлочейчек
	if (fp < 0) {
		std::cout << "cannt open file :" << file << std::endl;
	}


	pid_t process;	//Айди проги которую запустим

	//fork копирует текущий процесс только в дочу отдает 0 а родитель айди дочки
	if ((process = fork()) == -1) {	//Получаем айди процесса если прога не запустилась значит у нее фйди невалидный тоесть -1
		throw std::runtime_error("fork failed");	//Типа мы достаточно окультурились чтоб бросать обшибки
	} else if (process == 0) {	//Проверка на дочку (У дочки айди 0)
		dup2(fp, 1);	//Перенаправляем вывод дочки в файл (0 - stdin, 1 - stdout) как гениязавры
		//exec - заменяет фул прогру другой
		execvp(program.c_str(), args_c);	//Толкаем в дочь свою программу(мытья послуды или чего еще полезного) с аргументами естесна
		//Если exec выполнился то до этого места та прога не дойдет(она уже заменилась)
		//А если он не выполнился то сюда дойдет и по такому случаю бросим ошибку от дочки
		throw std::runtime_error("exec failed with "+program+args);	//Типа мы достаточно окультурились чтоб бросать обшибки

	}


	wait(NULL);	//Ждем дочку точнее то что в ней зародилось
	close(fp);
		


	return 0;
}
