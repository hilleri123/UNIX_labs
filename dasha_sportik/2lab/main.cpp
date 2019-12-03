#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <utility>
#include <sstream>
#include <iterator>
#include <unistd.h>

#include <fcntl.h>

//#include <sys/dos.h>

#include <sys/io.h>
#include <sys/types.h>
#include <sys/wait.h>


//Хотим 1 аргументом прогу последним файл
//Все что между аргументы проги
int main(int argc, char *argv[]) {
	using str=std::string;	//Хоти псевдоним для строки в 3 буквы вместо дофига


	//if (argc < 3) {	//Имя этой проги + имя проги что запустим + файл куда = минимум 3
		//throw std::invalid_argument("There's must be program name and output file");	//Типа мы достаточно окультурились чтоб бросать обшибки
		//return 1;
	//}





	str program_0 = "";	//Имя программулины
	str program_1 = "";	//Имя программулины

	str args_0;	//Аргументы
	str args_1;	//Аргументы

	bool arg0 = true;

	int time = 0;	//Время через которое запустим sec
	bool pos = false;	//Если тру то дождемся первой дочки если нет то ждать будем сразу две дочки

	int rez;
	while ((rez = getopt(argc, argv, "p:P:a:A:t:w")) > 0) {
			switch (rez) {
			case 'p': {
				program_0 = str(optarg);
				break;
			}
			case 'P': {
				program_1 = str(optarg);
				break;
			}
			case 'a': {
				args_0 = str(optarg);
				break;
			}
			case 'A': {
				args_1 = str(optarg);
				break;
			}
			case 't': {
				time = atoi(optarg);
				break;
			}
			case 'w': {
				pos = true;
				break;
			}
			}
	}
	//std::cout << program_0 << " " << args_0 << " // " << program_1 << " " << args_1 << " t " << time << std::endl;


	args_0 = program_0 + " " + args_0;
	args_1 = program_1 + " " + args_1;


	std::stringstream ss_0(args_0);
	std::istream_iterator<str> begin_0(ss_0);
	std::istream_iterator<str> end_0;
	std::vector<str> vec_0(begin_0, end_0);

	std::stringstream ss_1(args_1);
	std::istream_iterator<str> begin_1(ss_1);
	std::istream_iterator<str> end_1;
	std::vector<str> vec_1(begin_1, end_1);

	char** args_c_0 = static_cast<char**>(malloc(sizeof(const char*)*(vec_0.size())));	//Аргументы дочки маминой подруги
	char** args_c_1 = static_cast<char**>(malloc(sizeof(const char*)*(vec_1.size())));	//Аргументы дочки маминой подруги

	for (auto i = vec_0.begin(); i < vec_0.end(); i++)
		args_c_0[i-vec_0.begin()] = const_cast<char*>(i->c_str());

	for (auto i = vec_1.begin(); i < vec_1.end(); i++)
		args_c_1[i-vec_1.begin()] = const_cast<char*>(i->c_str());


	pid_t process_0;	//Айди проги которую запустим

	//fork копирует текущий процесс только в дочу отдает 0 а родитель айди дочки
	if ((process_0 = fork()) == -1) {	//Получаем айди процесса если прога не запустилась значит у нее фйди невалидный тоесть -1
		throw std::runtime_error("fork failed");	//Типа мы достаточно окультурились чтоб бросать обшибки
	} else if (process_0 == 0) {	//Проверка на дочку (У дочки айди 0)
		//exec - заменяет фул прогру другой
		execvp(program_0.c_str(), args_c_0);	//Толкаем в дочь свою программу(мытья послуды или чего еще полезного) с аргументами естесна
		//Если exec выполнился то до этого места та прога не дойдет(она уже заменилась)
		//А если он не выполнился то сюда дойдет и по такому случаю бросим ошибку от дочки
		throw std::runtime_error("exec failed with "+program_0+args_0);	//Типа мы достаточно окультурились чтоб бросать обшибки
	}

	if (pos) {	//Ждем или нет
		waitpid(process_0, NULL, 0);	//Ждем дочку точнее то что в ней зародилось
	}
	std::cout << "sleep " << time << std::endl;
	sleep(time);
	std::cout << "unsleep" << std::endl;
		
	pid_t process_1;	//Айди проги которую запустим

	//fork копирует текущий процесс только в дочу отдает 0 а родитель айди дочки
	if ((process_1 = fork()) == -1) {	//Получаем айди процесса если прога не запустилась значит у нее фйди невалидный тоесть -1
		throw std::runtime_error("fork failed");	//Типа мы достаточно окультурились чтоб бросать обшибки
	} else if (process_1 == 0) {	//Проверка на дочку (У дочки айди 0)
		//exec - заменяет фул прогру другой
		execvp(program_1.c_str(), args_c_1);	//Толкаем в дочь свою программу(мытья послуды или чего еще полезного) с аргументами естесна
		//Если exec выполнился то до этого места та прога не дойдет(она уже заменилась)
		//А если он не выполнился то сюда дойдет и по такому случаю бросим ошибку от дочки
		throw std::runtime_error("exec failed with "+program_1+args_1);	//Типа мы достаточно окультурились чтоб бросать обшибки
	}

	wait(NULL);	//Ждем дочкек или дочку нам все равно сколько их там

	sleep(1);

	return 0;
}
