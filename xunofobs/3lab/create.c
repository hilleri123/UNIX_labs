#include <stdio.h>

#include <fcntl.h>

#include <sys/errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#include "functions.h"

int
create(key_t key)
{
	int msqid;
	msqid = msgget(key, IPC_CREAT | 0660);

	if (msqid < 0) {
		fprintf(stderr, "msgget: %s\n", strerror(errno));
	}

	return msqid;
}
