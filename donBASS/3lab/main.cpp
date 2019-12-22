#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <utility>
#include <sstream>
#include <iterator>
#include <unistd.h>

#include <fcntl.h>

#include <aio.h>
#include <signal.h>
#include <string.h>

//#include <sys/dos.h>

//#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>



static void 
aio_handler(int sig, siginfo_t *si, void* data)
{
	//std::cout << "write" << std::endl;
	struct aiocb* ptr = static_cast<struct aiocb*>(si->si_value.sival_ptr);
	//std::cout << "ptr " << ptr->aio_nbytes << " " << ptr->aio_fildes << " " << ptr->aio_offset << std::endl;
	//std::cout << "write '" << std::get<0>(*static_cast<std::pair<struct aiocb, struct aiocb> *>(data)).aio_buf << "'" << std::endl;
	//std::pair<struct aiocb*, struct aiocb*> pair = *static_cast<std::pair<struct aiocb*, struct aiocb*>*>(data);
	//std::cout << "pair" << std::endl;
	//std::cout << "err " << strerror(aio_error(&std::get<0>(pair))) << std::endl;
	//std::cout << "red " << aio_return(ptr) << std::endl;
	//std::cout << "red " << aio_return(std::get<0>(pair)) << std::endl;
	//std::cout << "red " << aio_return(&std::get<0>(pair)) << " buf '" << static_cast<char*>(const_cast<void*>(std::get<0>(pair).aio_buf)) << "'" << std::endl;
	//std::cout << "'";
	//for (std::size_t j = 0; j < std::get<0>(pair).aio_nbytes; j++)
		//std::cout << static_cast<char*>(const_cast<void*>(std::get<0>(pair).aio_buf))[j];
	//std::cout << "'" << std::endl;;
	//std::cout << "fd " << std::get<1>(pair)->aio_fildes << std::endl;
	//std::cout << "buf size " << ptr->aio_nbytes << " buf '" << ptr->aio_buf  << "'" << std::endl;
	//std::cout << "fd " << ptr->aio_fildes << std::endl;
	//std::cout << "sync" << std::endl;
	//aio_fsync(O_SYNC, std::get<0>(pair));
	aio_fsync(O_SYNC, ptr);
	//std::cout << "sync end" << std::endl;
	//if (std::get<1>(pair)->aio_fildes >= 0) {
		//if (aio_write(std::get<1>(pair)) < 0)
	if (ptr->aio_fildes >= 0) {
		if (aio_write(ptr) < 0)
			std::cout << "aio_write faild" << std::endl;
	} else {
		std::cout << "bad fd" << std::endl;
	}
	//std::cout << "write end" << std::endl;
}


//Хотим 1 аргументом прогу последним файл
//Все что между аргументы проги
int main(int argc, char *argv[]) {
	using str=std::string;	//Хоти псевдоним для строки в 3 буквы вместо дофига


	//if (argc < 3) {	//Имя этой проги + имя проги что запустим + файл куда = минимум 3
		//throw std::invalid_argument("There's must be program name and output file");	//Типа мы достаточно окультурились чтоб бросать обшибки
		//return 1;
	//}



	str target_file = "";

	std::vector<struct aiocb *> files;
	std::vector<std::size_t> offs;

	//std::vector<char*> bufs;

	std::size_t seek = 0;
	std::size_t size = 0;
	std::size_t off = 0;


	struct sigaction sa;

	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);

	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa.sa_sigaction = aio_handler;


	if (sigaction(SIGUSR1, &sa, NULL) == -1)
    		throw std::runtime_error("sigaction");

	int rez;
	while ((rez = getopt(argc, argv, "f:p:s:t:c:")) > 0) {
			switch (rez) {
			case 'f': {
				struct aiocb* tmp_io = new struct aiocb;
				tmp_io->aio_fildes = open(optarg, O_RDONLY);
				//std::cout << cur_file.aio_fildes << std::endl;
				if (tmp_io->aio_fildes < 0)
					std::cout << "bad discriotor" << std::endl;
        			tmp_io->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        			tmp_io->aio_sigevent.sigev_signo = SIGUSR1;
				std::size_t loc_size = size;
				if (loc_size == 0) {
					struct stat st;
					fstat(tmp_io->aio_fildes, &st);
					loc_size = st.st_size;
					std::cout << loc_size << std::endl;
				}
				char* tmp = new char[loc_size+1];
				tmp[size] = 0;
				//bufs.push_back(tmp);

				tmp_io->aio_buf = static_cast<void*>(tmp);
				tmp_io->aio_nbytes = loc_size;
				tmp_io->aio_offset = seek;

				offs.push_back(off);
				files.push_back(tmp_io);
				//std::cout << "f" << std::endl;
				break;
			}
			case 'p': {
				seek = atoi(optarg);
				break;
			}
			case 'c': {
				off = atoi(optarg);
				break;
			}
			case 's': {
				size = atoi(optarg);
				//std::cout << "size " << size << std::endl;
				break;
			}
			case 't': {
				target_file = str(optarg);
				break;
			}
			}
	}

	//std::cout << "size " << files.size() << std::endl;

	struct aiocb ** list = new struct aiocb*[files.size()];
	struct aiocb ** target = new struct aiocb*[files.size()];

	for (auto i = 0; i < files.size(); i++) {
		list[i] = files.at(i);
		target[i] = new struct aiocb;
		target[i]->aio_nbytes = list[i]->aio_nbytes;
		target[i]->aio_offset = offs.at(i);
		target[i]->aio_buf = list[i]->aio_buf;
		target[i]->aio_fildes = open(target_file.c_str(), O_WRONLY);
		//std::cout << "!!fd" << target[i]->aio_fildes << std::endl;
		//std::get<1>(*i)->aio_fildes = open(target_file.c_str(), O_WRONLY);
		list[i]->aio_sigevent.sigev_value.sival_int = i;
		list[i]->aio_sigevent.sigev_value.sival_ptr = target[i];
		//std::cout << "list " << list[i]->aio_nbytes << " " << list[i]->aio_fildes << " " << list[i]->aio_offset << std::endl;
		//std::cout << "target " << target[i]->aio_nbytes << " " << target[i]->aio_fildes << " " << target[i]->aio_offset << std::endl;
		//std::cout << "read" << std::endl;
		if (aio_read(list[i]) < 0)
			std::cout << "aio_read faild" << std::endl;
	}
	//std::cout << "end for" << std::endl;

	aio_suspend(list, files.size(), NULL);

	//aio_suspend(target, files.size(), NULL);
	//sleep(10);
	//pause();
	//std::cout << "unpause" << std::endl;

	//std::cout << "returns" << std::endl;
#if 0
	for (auto i = files.begin(); i < files.end(); i++) {
		std::cout << "read " << aio_return(&std::get<0>(*i)) << " buf " << std::get<0>(*i).aio_buf << std::endl;
		std::cout << "'";
		for (std::size_t j = 0; j < std::get<0>(*i).aio_nbytes; j++)
			std::cout << static_cast<char*>(const_cast<void*>(std::get<0>(*i).aio_buf))[j];
		std::cout << "'" << std::endl;;
		//std::cout << "write " << aio_return(&std::get<1>(*i)) << std::endl;
	}
#endif

	//for (auto i = bufs.begin(); i < bufs.end(); i++) {
		//delete *i;
	//}

	return 0;
}
