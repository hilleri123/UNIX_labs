#include <iostream>
#include <string>
#include <array>
#include <utility>
#include <unistd.h>

#include <fcntl.h>

#include <ext/stdio_filebuf.h>

#include <errno.h>
#include <sys/io.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {

	if (argc != 4)
		throw std::invalid_argument("Get "+std::to_string(argc-1)+"arguments but expected 3");

	int in_fd[2];
	int out_fd[2];


	if (pipe(in_fd) < 0) {
		throw std::runtime_error("Pipe failed");
	}
	if (pipe(out_fd) < 0) {
		throw std::runtime_error("Pipe failed");
	}
	//std::cout << in_fd[0] << " " << in_fd[1] << " " << out_fd[0] << " " << out_fd[1] << std::endl;

	pid_t pid = fork();
	if (pid < 0) {
		throw std::runtime_error("Fork failed");
	} else if (pid == 0) {
		dup2(in_fd[0], 0);
		dup2(out_fd[1], 1);
		close(in_fd[1]);
		close(out_fd[0]);
		//std::cout << "hi" << std::endl;
		int status = execlp("./_reshala", "_reshala", nullptr);
		//int status = execlp("cat", "cat", NULL);
		throw std::runtime_error("Exec failed with "+std::to_string(status));
	}
	close(in_fd[0]);
	close(out_fd[1]);


	std::array<char, 256> buf;
	std::string tmp;
	std::string data = "";
	for (int i = 1; i < argc; i++)
		data += argv[i];
	std::string result;

	int bytes = 0;
	if ((bytes = write(in_fd[1], data.c_str(), data.size()+1)) < 0) {
		throw std::runtime_error("Write failed wrote "+std::to_string(bytes)+" but expected "+std::to_string(data.size()));
	}

	//bytes = read(fd[1], buf.data(), buf.size());
	//std::cout << bytes << std::endl;
	//result = buf.data();
	//std::cout << result << std::endl;

	//while (!eof(fd[1])) {
	//std::cout << "while" << std::endl;
	while ((bytes = read(out_fd[0], buf.data(), buf.size()-1)) > 0) {
	//__gnu_cxx::stdio_filebuf<char> filebuf(fd[1], std::ios::in);
	//std::istream is(&filebuf);
	//while (getline(is, tmp)) {
	//while ((bytes = getline(buf.data(), buf.size(), fd[1])) > 0) {
		//std::cout << "red " << bytes << " '" << buf[bytes-1] << "'" << std::endl;
		//if (read(fd[1], buf.data(), buf.size()) > 0) 
		result += buf.data();
		//result += tmp;
		//std::cout << result << std::endl;

	}
	if (bytes < 0) {
		throw std::runtime_error("Read failed with status "+std::to_string(errno));
	}
	//std::cout << "pre wait" << std::endl;
	waitpid(pid, NULL, 0);
	//std::cout << "post wait" << std::endl;
	close(in_fd[1]);
	close(out_fd[0]);


	//if (pclose(pipe) != 0)
		//throw std::runtime_error("pclose failed");
		

	std::cout << result << std::endl;

	return 0;
}
