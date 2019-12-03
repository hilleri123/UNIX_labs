#include <stdio.h>
#include <rpc/rpc.h>

#include "del.h"



char **
//del_1(char ** t, struct svc_req *rpstp)
del_1(char ** t, CLIENT * clnt)
{
	printf("%s\n", *t);
	return t;
}

char **
del_1_svc(char** t, struct svc_req *svc)
{
	printf("%s\n", *t);
	CLIENT * clnt;
	return del_1(t, clnt);
}
