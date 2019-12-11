#include <stdio.h>

#include <fcntl.h>

#include <sys/errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>


#include "functions.h"

//bool
int
delete_q(int msqid)
{
	msqid = msgctl(msqid, IPC_RMID, NULL);

	if (msqid < 0) {
		fprintf(stderr, "msgctl: %s\n", strerror(errno));
		//return false;
		return 1;
	}

	//return true;
	return 0;
}
