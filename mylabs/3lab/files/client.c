#include <stdio.h>
#include <rpc/rpc.h>

#include "del.h"


int
main(int argc, char* argv[])
{
	//if (argc < 3)
		//return 1;
	if (argc < 3)
		return 1;
	//CLIENT* clnt = clnt_create(argv[1], DELPROG, DELVER, "tcp");
	CLIENT* clnt = clnt_create("localhost", DELPROG, DELVER, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror(argv[1]);
		return 1;
	}
	//pol* msg = {argv[1], argv[2]};
	pol msg = {argv[1], argv[2]};
	//printf("\n");
	char** str = del_1(&msg, clnt);
	//printf("%s\n", *del_1(&argv[2], clnt));
	//printf("\n");
	printf("%s\n", *str);
	return 0;
};

