#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <unistd.h>

#include <pthread.h>
#include <stdlib.h>
#include <signal.h>

#include <fcntl.h>

#include <sys/time.h>


//Библа написана на Си поэтому нам нужны СИшные сигнатуры функций
//И как следствие мы объявляем extern "C"
//Сигнатура - типа уневерсальное имя
extern "C" {
#include "functions.h"
}


static void 
print_m(union sigval sv)
{

	//std::cout << "1!!" << std::endl;
	mqd_t qid = *((mqd_t*) sv.sival_ptr);

	const char* tmp = read_q(qid);
	if (tmp != nullptr) {
		std::string msg(tmp);
		std::cout << msg << std::endl;
	}
	//exit(EXIT_SUCCESS);
	struct sigevent sev;

	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = print_m;
	sev.sigev_notify_attributes = NULL;
	sev.sigev_value.sival_ptr = sv.sival_ptr;

	if (mq_notify(qid, &sev) < 0)
		throw std::runtime_error("notify queue");

}


int main(int argc, char *argv[]) {


	mqd_t* qid = new mqd_t(-1);

	if ((*qid = open_q(NAME)) < 0) {
		throw std::runtime_error("open queue");
	}

	struct sigevent sev;

	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = print_m;
	sev.sigev_notify_attributes = NULL;
	sev.sigev_value.sival_ptr = qid;

	if (mq_notify(*qid, &sev) < 0)
		throw std::runtime_error("notify queue");

	


#if 0
	mqd_t tmp_qid = 
	int n = 0;
	while (n >= 0) {
		struct mq_attr *attr = new struct mq_attr;
		mq_getattr(*qid, attr);
		char* buf = new char[attr->mq_msgsize];
		n = mq_receive(*qid, buf, attr->mq_msgsize, NULL);
		std::cout << buf << std::endl;
	}
#endif
	//sleep(10000);
	//std::cout << "pause" << std::endl;
	pause();

	delete_q(*qid);


	return 0;
}
