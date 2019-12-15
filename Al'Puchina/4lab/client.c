#include <stdio.h>
#include <rpc/rpc.h>

#include "del.h"


int
main(int argc, char* argv[])
{
	//Содержание argv : 0 - Cleint; 1 - имя файла; 2 - позиция курсора; 3 - комманда(r, w); 4 - если r, то сколько прочитать, если w, то что записать
	//argc = 1+1+1+1+1 = 5
	if (argc != 5) {
		//Нулевой аргумент имя программы
		printf("Expected 4 args but got %d\n", argc-1);
		return 1;
	}
	CLIENT* clnt = clnt_create("localhost", DELPROG, DELVER, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror(argv[1]);
		return 1;
	}

	//file seek size com s
	pol msg;
	msg.file = argv[1];
	msg.seek = atoi(argv[2]);
	msg.com = argv[3];

	if (argv[3][0] == 'r')
		msg.size = atoi(argv[4]);
	if (argv[3][0] == 'w')
		msg.s = argv[4];


	//printf("start\n");
	char** res = del_1(&msg, clnt);
	//printf("finish %d\n", *res);

	if (atoi(*res) < 0) {
		printf("Server Error %d\n", atoi(*res));
		return atoi(*res);
	}

	if (argv[3][0] == 'r')
		printf("%s\n", *res);

	return 0;
};

