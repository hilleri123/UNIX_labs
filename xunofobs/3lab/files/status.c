#include <stdio.h>

#include <fcntl.h>

#include <sys/errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#include "functions.h"

int
status(int msqid)
{

	struct msqid_ds status;
	if (msgctl(msqid, IPC_STAT, &status) < 0) {
		fprintf(stderr, "msgctl: %s\n", strerror(errno));
		return 1;
	}
	printf("cuid %d\n", status.msg_perm.cuid);
	printf("cgid %d\n", status.msg_perm.cgid);
	printf("uid %d\n", status.msg_perm.uid);
	printf("gid %d\n", status.msg_perm.gid);
	printf("mode %d\n", status.msg_perm.mode);
	printf("qnum %d\n", status.msg_qnum);

	return 0;
}
