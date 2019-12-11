#include <stdio.h>
#include <string.h>

#include <fcntl.h>

#include <sys/errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#include "functions.h"

int
push(int msqid, const char * msg)
{
	//if (msgsnd(msqid, (void *)msg, strlen(msg), IPC_NOWAIT) < 0) {
	if (msgsnd(msqid, (void *)msg, sizeof(msg), IPC_NOWAIT) < 0) {
		fprintf(stderr, "msgsnd: %s\n", strerror(errno));
		return 1;
	}

	return 0;
}
