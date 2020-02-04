#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <iostream>



int
main(int argc, char* argv[])
{
	if (argc < 3)
		return 1;

	struct sockaddr_in serv_addr; 

	//sd - socket descriptor
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd < 0) {
		perror("socket ");
		return 1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);

	
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		perror("inet_pton ");
		return 1;
	}

	if(connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect ");
		return 1;
	}

	const int N = 256;
	int n = 0;
	char buf[N+1];

	std::string first(argv[1]);
	std::string second(argv[2]);

	//std::cout << argv[1] << "(" << strlen(argv[1]) << ") " << argv[2] << "(" << strlen(argv[2]) << ")" << std::endl;

	if (write(sd, first.c_str(), strlen(first.c_str())) < 0) {
		perror("write");
		return 1;
	}

	if ((n = read(sd, buf, sizeof(buf)-1)) >= 0) {
		//buf[n] = 0;
		//std::cout << buf << std::endl;
	} else {
		perror("read");
		return 1;
	}


	if (write(sd, second.c_str(), strlen(second.c_str())) < 0) {
		perror("write");
		return 1;
	}

	if ((n = read(sd, buf, sizeof(buf)-1)) >= 0) {
		buf[n] = 0;
		std::cout << buf << std::endl;
	} else {
		perror("read");
		return 1;
	}

	return 0;
};

