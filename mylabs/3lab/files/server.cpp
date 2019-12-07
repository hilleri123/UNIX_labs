#include <cstring>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <rpc/rpc.h>

#include "del.h"
#include "poly.h"




char **
//del_1(char ** t, struct svc_req *rpstp)
//del_1(char ** t, CLIENT * clnt)
del_1(pol * p, CLIENT * clnt)
{
	char* f = p->first;
	char* s = p->second;
	std::string div(f);
	std::string del(s);
	std::string out;

	Polynom a(div);
	//a.print();

	Polynom b(del);
	//b.print();

	/*
	Polynom c = a + b;
	printf("+\n");
	c.print();
	c = a - b;
	printf("-\n");
	c.print();
	c = a * b;
	printf("*\n");
	c.print();
	auto d = a / b;
	printf("/\n");
	std::get<0>(d).print();
	std::get<1>(d).print();
	*/

	out = "chast '" + std::get<0>(d).string() + "' ost '" + std::get<1>(d).string() + "'";

	char** result = new char*;
	*result = new char[out.length()+1];
	strcpy(*result, out.c_str());
	//printf("%s\n", *result);
	return result;
}

char **
//del_1_svc(char** t, struct svc_req *svc)
del_1_svc(pol * t, struct svc_req *svc)
{
	//printf("%s\n", *t);
	CLIENT * clnt;
	return del_1(t, clnt);
}
