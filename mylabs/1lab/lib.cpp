//#include <iostream>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

extern "C" {
	

void run(void) {
	//std::cout << "Hello, world!" << std::endl;
	printf("Hello, world!\n");
}


}

extern "C" {
	

void user(void) {
	//std::cout << "Hello, world!" << std::endl;
	uid_t uid = geteuid();
	struct passwd *pw = getpwuid(uid);
	if (pw)
		printf("Hello, %s!\n", pw->pw_name);
	else
		printf("Hello, nameless!\n");
}


}

