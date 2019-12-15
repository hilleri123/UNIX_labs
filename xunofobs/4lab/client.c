#include <stdio.h>
#include <rpc/rpc.h>

#include "sender.h"


int
main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("Expected 2 args but got %d\n", argc-1);
		return 1;
	}
	CLIENT* clnt = clnt_create("localhost", SENDERPROG, SENDERVER, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror(argv[1]);
		return 1;
	}



	//printf("start\n");
	if (argv[2][0] == 's') {
		short i16;
		long i32;
		float f32;
		double f64;

		printf("i16: ");
		scanf("%d", &i16);
		printf("i32: ");
		scanf("%ld", &i32);
		printf("f32: ");
		scanf("%f", &f32);
		printf("f64: ");
		scanf("%lf", &f64);

		num b = {i16, i32, f32, f64};
		//printf("Server send short: %d long: %d float: %f double: %f\n", b.i16, b.i32, b.f32, b.f64);

		s_num a = {argv[1], b};
		//printf("send\n");
		int* res = sender_1(&a, clnt);
	
		if (*res < 0) {
			printf("Server Error %d\n", *res);
			return *res;
		}
	} else if (argv[2][0] == 'g') {
		num* a = getter_1(&argv[1], clnt);
		printf("Server send short: %d long: %d float: %f double: %f\n", a->i16, a->i32, a->f32, a->f64);
	}
	//printf("finish\n");


	return 0;
};

