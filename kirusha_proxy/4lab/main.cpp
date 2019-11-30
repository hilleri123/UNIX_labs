
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include <iostream>
#include <functional>
#include <cstring>
#include <regex>

int pid;
int sig;
timer_t id;

void signal_handler(int _sig, siginfo_t *si, void *uc) {
    kill(pid, sig);
    std::cout << "kill " << sig << " to " << pid << std::endl;
}

void del_handler(int s) {
    timer_delete(id);
    exit(1);
}

int main(int argc, char *argv[]) {
    	int freq_secs;

    	struct tm calendar_time;
	


    	//if (argc < 4 || argc > 6) {
    	if (argc < 4 || argc > 5) {
        	//throw std::invalid_argument(std::string(argv[0]) + std::string(" <pid> <sig> <time>(ss:mm:hh:DD:MM:YYYY/seconds) option -r/-s"));
        	throw std::invalid_argument(std::string(argv[0]) + std::string(" <pid> <sig> <time>(ss:mm:hh:DD:MM:YYYY) option -r(recursive)"));
    	}

    	pid = atoi(argv[1]);
    	sig = atoi(argv[2]);
	
	//собираем таймер
    	struct sigevent sev;
    	sev.sigev_notify = SIGEV_SIGNAL;
    	sev.sigev_signo = SIGUSR1;
    	sev.sigev_value.sival_ptr = &id;
    	if (timer_create(CLOCK_REALTIME, &sev, &id) == -1)
        	throw std::runtime_error("timer_create");
	
	//хз хачем. Скорее всего чистит таймер и завершает работу этой проги
    	signal(SIGINT, del_handler);

	//привязываем сигнал
    	struct sigaction sa;
    	sa.sa_flags = SA_SIGINFO;
    	sa.sa_sigaction = signal_handler;
    	sigemptyset(&sa.sa_mask);
    	if (sigaction(SIGUSR1, &sa, NULL) == -1)
        	throw std::runtime_error("sigaction");
	
	
	
    	struct itimerspec its;
	int flags = 0;
	bool r = false;
	long time_s = 0;
	//bool s = false;
	if (argc >= 5) {
		r = std::string(argv[4]) == "-r" || r;
		//s = std::string(argv[4]) == "-s" || s;
	}
	//if (argc >= 6) {
		//r = std::string(argv[5]) == "-r" || r;
		//s = std::string(argv[5]) == "-s" || s;
	//}

	std::string input(argv[3]);

	std::regex spliter(":");
	std::sregex_token_iterator
		first{input.begin(), input.end(), spliter, -1},
		last;
	if (first != last) {
        	calendar_time.tm_sec = std::stoi(*first);
		//std::cout << "s " << std::stoi(*first) << std::endl;
		first++;
	} else
        	calendar_time.tm_sec = 0;
	if (first != last) {
        	calendar_time.tm_min = std::stoi(*first);
		//std::cout << "m " << std::stoi(*first) << std::endl;
		first++;
	}  else
        	calendar_time.tm_min = 0;
	if (first != last) {
        	calendar_time.tm_hour = std::stoi(*first);
		//std::cout << "h " << std::stoi(*first) << std::endl;
		first++;
	} else
        	calendar_time.tm_hour = 3;
	if (first != last) {
        	calendar_time.tm_mday = std::stoi(*first);
		//std::cout << "d " << std::stoi(*first) << std::endl;
		first++;
	} else
        	calendar_time.tm_mday = 1;
	if (first != last) {
        	calendar_time.tm_mon = std::stoi(*first) - 1;
		//std::cout << "m " << std::stoi(*first) << std::endl;
		first++;
	} else
        	calendar_time.tm_mon = 0;
	if (first != last) {
        	calendar_time.tm_year = std::stoi(*first) - 1900;
		//std::cout << "y " << std::stoi(*first) << std::endl;
		first++;
	} else
        	calendar_time.tm_year = 70;

        calendar_time.tm_isdst = -1;

	
        its.it_value.tv_sec = static_cast<long>(mktime(&calendar_time));
        its.it_value.tv_nsec = 0;

	if (r) {
        	its.it_interval.tv_sec = its.it_value.tv_sec;
        	its.it_interval.tv_nsec = its.it_value.tv_nsec;
    	} else {
		flags = flags | TIMER_ABSTIME;
        	its.it_interval.tv_sec = 0;
        	its.it_interval.tv_nsec = 0;
	}

        if (timer_settime(id, flags, &its, NULL) == -1)
            	throw std::runtime_error("timer_settime");
	
	std::string b;
	std::cin >> b;
	//kill(id, 9);
}
