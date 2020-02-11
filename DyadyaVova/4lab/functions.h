
#pragma once

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

#include <sys/errno.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PROJ_ID 0x3333

//extern "C" {

	//key_t create_k(const char*);

	//int create_s(key_t, int);

	//int delete_q(int);

	//int operation(int, long);

	//int push(int, long, const char*);

	//int status(int);
//}

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)


#define CALL_FUNC(result, func, ...) \
do { \
	result = func(__VA_ARGS__); \
	if (result < 0) { \
		fprintf(stderr, TOSTRING(func)" %s\n", strerror(errno)); \
	} \
} while (0)

