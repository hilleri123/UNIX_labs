#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <cstring>


#include "poly.h"

int main(int argc, char *argv[])
{
	//ld - listen discriptor
	//cd - connection descriptor
	int ld = 0, cd = 0;
	struct sockaddr_in serv_addr; 

	const int N = 256;
	char buf[N+1];
	time_t ticks; 

	ld = socket(AF_INET, SOCK_STREAM, 0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000); 

	bind(ld, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

	listen(ld, 10); 

	std::string hello = "send moreee";

	while(1)
	{
		cd = accept(ld, (struct sockaddr*)nullptr, NULL); 

		//ticks = time(NULL);
		//snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
		//write(cd, buf, strlen(buf)); 
		read(cd, buf, N);
		//std::cout << buf << std::endl;
		Polynom a = Polynom(std::string(buf));
		memset(&buf, 0, N+1);

		write(cd, hello.c_str(), strlen(hello.c_str()));

		read(cd, buf, N);
		//std::cout << buf << std::endl;
		Polynom b = Polynom(std::string(buf));
		memset(&buf, 0, N+1);

		Polynom c = a * b;
		//c.print();
		write(cd, c.string().c_str(), strlen(c.string().c_str()));

		close(cd);
		sleep(1);
	}
}
