#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <iostream>

//Запуск будет типа
// ./Server - В другом терминале
// ./Client 4x^4+1x^3+7x^1 5x^9


int
main(int argc, char* argv[])
{
	//Мы хотим 2 плоинома + 1 это имя проги и того 3
	if (argc != 3)
		return 1;

	//Дефолт структоура для сокетов
	struct sockaddr_in serv_addr; 

	//sd - socket descriptor
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd < 0) {
		//perror пишет (ее аргемент) + (ошибку)
		perror("socket ");
		return 1;
	}
	//Заполняем ее и ставим порт 5000 потому что он должен быть больше 3к (вроде)
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);

	
	//связываемся с адресом тут это локаль
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		perror("inet_pton ");
		return 1;
	}

	//Пытаемся законектисться
	if(connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect ");
		return 1;
	}

	const int N = 256;
	int n = 0;
	char buf[N+1];

	std::string first(argv[1]);
	std::string second(argv[2]);

	//std::cout << argv[1] << "(" << strlen(argv[1]) << ") " << argv[2] << "(" << strlen(argv[2]) << ")" << std::endl;

	//Пишем туда первый полином
	if (write(sd, first.c_str(), strlen(first.c_str())) < 0) {
		perror("write");
		return 1;
	}

	//Читаем что первый полином приняли а то если мы отправим сразу второй они могут слиться в один
	if ((n = read(sd, buf, sizeof(buf)-1)) >= 0) {
		//buf[n] = 0;
		//std::cout << buf << std::endl;
	} else {
		perror("read");
		return 1;
	}


	//Пишем туда второй полином
	if (write(sd, second.c_str(), strlen(second.c_str())) < 0) {
		perror("write");
		return 1;
	}

	//Читаем и выводим ответ
	if ((n = read(sd, buf, sizeof(buf)-1)) >= 0) {
		buf[n] = 0;
		std::cout << buf << std::endl;
	} else {
		perror("read");
		return 1;
	}

	return 0;
};

