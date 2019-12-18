#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

const char*
pull(int msqid, long type)
{
	//struct msgbuf *msg = (struct msgbuf)malloc(sizeof(struct msgbuf));
	char *text = (char*)malloc(sizeof(char)*MSGSZ);
	//struct msgbuf msg;
	message_buf msg;

	if (msgrcv(msqid, (void *)&msg, MSGSZ, type, IPC_NOWAIT | MSG_NOERROR) < 0) {
		fprintf(stderr, "msgrcv: %s\n", strerror(errno));
		return NULL;
	}
	strcpy(text, msg.mtext);

	return text;
}


int
push(int msqid, long type, const char * text)
{
	//if (msgsnd(msqid, (void *)msg, strlen(msg), IPC_NOWAIT) < 0) {
	message_buf msg;
	msg.mtype = type;
	strcpy(msg.mtext, text);
	if (msgsnd(msqid, (void *)&msg, MSGSZ, IPC_NOWAIT) < 0) {
		fprintf(stderr, "msgsnd: %s\n", strerror(errno));
		return 1;
	}

	return 0;
}


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
