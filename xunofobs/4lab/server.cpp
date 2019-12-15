#include <cstring>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <rpc/rpc.h>

#include <array>
#include <utility>
#include <fstream>
#include <unistd.h>

#include <fcntl.h>

#include <sys/io.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/wait.h>





#include "sender.h"


int *
sender_1(s_num * p, CLIENT * clnt)
{
	char* file = p->file;

	FILE* fd = fopen(file, "w");	//Откроем файлочейчек

	if (fd < 0) {
		std::cout << "cannt open file " << file << std::endl;
		return new int(-1);
	}
	
	XDR xhandle;
	char* buf = new char[sizeof(num)];
	xdrmem_create(&xhandle, buf, sizeof(num), XDR_ENCODE);

	if (xdr_num(&xhandle, &(p->num)) != true) {
		std::cout << "xdr_num failed" << std::endl;
		return new int(-2);
	}

	int size = xdr_getpos(&xhandle);

	if (fwrite(buf, 1, size, fd) < 0) {
		std::cout << "write failed" << std::endl;
		return new int(-3);
	}

	fclose(fd);


	return new int(0);
}

int *
sender_1_svc(s_num * t, struct svc_req *svc)
{
	CLIENT * clnt;
	return sender_1(t, clnt);
}



num *
getter_1(char ** f, CLIENT * clnt)
{

	FILE* fd = fopen(*f, "r");	//Откроем файлочейчек

	if (fd < 0) {
		std::cout << "cannt open file " << *f << std::endl;
		return new num;
	}
	num* res = new num;

	XDR xhandle;
	char* buf = new char[sizeof(num)];

	int size;
	if ((size = fread(buf, 1, sizeof(num), fd)) < 0) {
		std::cout << "read failed" << std::endl;
		return new num;
	}
	xdrmem_create(&xhandle, buf, size, XDR_DECODE);
	memset(res, 0, sizeof(num));

	if (xdr_num(&xhandle, res) != true) {
		std::cout << "xdr_num failed" << std::endl;
		return new num;
	}






	fclose(fd);
	return res;
}

num *
getter_1_svc(char ** t, struct svc_req *svc)
{
	CLIENT * clnt;
	return getter_1(t, clnt);
}















