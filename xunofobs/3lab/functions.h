#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#include <string.h>


//extern "C" {

	int create(key_t);

	//bool delete_q(int);
	int delete_q(int);

	//struct msgbuf* pull(int);
	const char* pull(int);

	int push(int, const char*);

	int status(int);
//}
