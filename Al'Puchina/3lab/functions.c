
#include "functions.h"

static
struct mq_attr*
crt_attr()
{
	struct mq_attr* attr = (struct mq_attr*)malloc(sizeof(struct mq_attr));
	attr->mq_flags = 0;
	attr->mq_maxmsg = 10;
	attr->mq_msgsize = 32;
	attr->mq_curmsgs = 0;

	return attr;
}



mqd_t
create(const char* name)
{
	mqd_t qid;
	qid = mq_open(name, O_CREAT | O_WRONLY, 0666, crt_attr());
	if (qid < 0) {
		qid = mq_open(name, O_WRONLY);
		if (qid < 0)
			fprintf(stderr, "mq_open: %s\n", strerror(errno));
	}
	return qid;
}

mqd_t
open_q(const char* name)
{
	mqd_t qid;
	qid = mq_open(name, O_RDONLY, 0666, crt_attr());
	if (qid < 0) {
		fprintf(stderr, "mq_open: %s\n", strerror(errno));
	}
	return qid;
}


//bool
mqd_t
delete_q(mqd_t qid)
{
	qid = mq_close(qid);
	if (qid < 0) {
		fprintf(stderr, "mq_close: %s\n", strerror(errno));
		return 1;
	}
	return 0;
}


const char*
read_q(mqd_t qid)
{
	//struct msgbuf *msg = (struct msgbuf)malloc(sizeof(struct msgbuf));
	struct mq_attr* attr = (struct mq_attr*)malloc(sizeof(struct mq_attr));
	long size = 256;
	
	if (mq_getattr(qid, attr) >= 0)
		size = attr->mq_msgsize;

	char *msg = (char*)malloc(sizeof(char)*size);


	if (mq_receive(qid, msg, size, NULL) < 0) {
		fprintf(stderr, "mq_receive: %s\n", strerror(errno));
		return NULL;
	}

	return msg;
}


mqd_t
write_q(mqd_t qid, const char * msg)
{
	//if (msgsnd(msqid, (void *)msg, strlen(msg), IPC_NOWAIT) < 0) {
	if (mq_send(qid, msg, strlen(msg)+1, 1) < 0) {
		fprintf(stderr, "mq_send: %s\n", strerror(errno));
		return 1;
	}

	return 0;
}
