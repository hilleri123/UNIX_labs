
#pragma once

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#include <string.h>

#define MSGSZ 256

//extern "C" {
typedef struct msg_buf {
	long mtype;
	char mtext[MSGSZ];
} message_buf;

	int create(key_t);

	//bool delete_q(int);
	int delete_q(int);

	//struct msgbuf* pull(int);
	const char* pull(int, long);

	int push(int, long, const char*);

	int status(int);
//}
