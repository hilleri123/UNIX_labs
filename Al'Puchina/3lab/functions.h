#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>

#include <sys/errno.h>
//#include <sys/ipc.h>
#include <sys/types.h>
//#include <sys/msg.h>
#include <mqueue.h>

#include "conf.h"

#ifndef NAME

#define NAME "/queue"

#endif

//extern "C" {

	mqd_t create(const char *);

	mqd_t open_q(const char *);

	//bool delete_q(mqd_t);
	mqd_t delete_q(mqd_t);

	//struct msgbuf* pull(mqd_t);
	const char* read_q(mqd_t);

	mqd_t write_q(mqd_t, const char*);

//}
