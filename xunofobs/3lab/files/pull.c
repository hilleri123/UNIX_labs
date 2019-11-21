#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>

#include <sys/errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>


#include "functions.h"

const char*
//struct msgbuf*
pull(int msqid)
{
	//struct msgbuf *msg = (struct msgbuf)malloc(sizeof(struct msgbuf));
	const char *msg = (const char*)malloc(sizeof(char)*256);

	if (msgrcv(msqid, (void *)msg, 256, 0, IPC_NOWAIT | MSG_NOERROR) < 0) {
		fprintf(stderr, "msgrcv: %s\n", strerror(errno));
		return NULL;
	}

	return msg;
}
