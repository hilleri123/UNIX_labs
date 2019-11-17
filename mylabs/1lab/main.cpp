#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include <dlfcn.h>


int main(int argc, char *argv[]) {
	using svec=std::vector<std::string>;
	using vec=std::vector<std::pair<void*, std::string>>;
	bool exit = false;
	std::string buf;
	vec libs;
	while (!exit) {
		char buff[256];
		std::cin.getline(buff, sizeof(buff));
		//std::cin >> buf;
		buf = std::string(buff);
		//std::cout << "'" << buf << "'" << std::endl;
		svec args;
		auto idx = buf.begin();
		for (auto i = buf.begin(); i < buf.end(); i++) {
			if (*i == ' ') {
				args.emplace_back(idx, i);
				idx = i + 1;
			}
		}
		args.emplace_back(idx, buf.end());
		if (*args.begin() == "a") {
			for (auto i = args.begin()+1; i < args.end(); i++) {
				std::cout << *i << std::endl;
				if (*(args.begin()) == "a") {
					auto lib = dlopen(i->c_str(), RTLD_LAZY);
					if (!lib) {
						std::cout << "cannot open lib " << *i << std::endl;
						std::cout << "dlerror :" << dlerror() << std::endl;
					} else 
						libs.push_back(std::make_pair(lib, *i));
				}
				dlerror();
			}
		} else if (*args.begin() == "l") {
			for (auto i = libs.begin(); i < libs.end(); i++) {
				std::cout << "'" << std::get<1>(*i) << "' ";
			}
			std::cout << std::endl;
		} else if (*args.begin() == "f") {
			void* lib = nullptr;
			std::string name;
			for (auto i = args.begin()+1; i < args.end(); i++) {
				if (lib) {
		       			auto fun = (void (*)(void))dlsym(lib, i->c_str());
					if (!fun) {
  						std::cout << "cannot load function '" << *i << "' from " << name << std::endl;
						std::cout << "dlerror :" << dlerror() << std::endl;
					} else
  						fun();
					dlerror();
				} else {
					name = *i;
					for (auto j = libs.begin(); j < libs.end(); j++) {
						if (std::get<1>(*j) == *i) {
							lib = std::get<0>(*j);
							break;
						}
					}
					if (!lib) {
						std::cout << *i << " isn't opened" << std::endl;
					}
				}
			}
		} else if (*args.begin() == "c") {
			for (auto i = args.begin()+1; i < args.end(); i++) {
				auto del = libs.end();
				void* lib = nullptr;
				for (auto j = libs.begin(); j < libs.end(); j++) {
					if (std::get<1>(*j) == *i) {
						lib = std::get<0>(*j);
						break;
					}
				}
				if (!lib) {
					std::cout << *i << " isn't opened" << std::endl;
				} else {
					dlclose(lib);
					libs.erase(del);
				}
			}
			//if (fun_idx > lib_idx) {
				////std::cout << "c str " << fun_idx->c_str() << " " << lib_idx->c_str() << std::endl;
				////void (*fun)(void);
		       		//auto fun = (void (*)(void))dlsym(lib, fun_idx->c_str());
				//if (!fun) {
  					//std::cout << "cannot load function '" << *fun_idx << "' from " << *lib_idx << std::endl;
					//std::cout << "dlerror :" << dlerror() << std::endl;
					//} else
  				//fun();
				//dlerror();
				//fun_idx = vargv.begin();
			//}
			//dlclose(lib);
		} else if (*args.begin() == "exit") {
			exit = true;
			//std::cout << "exit " << exit << std::endl;
		}
	}
	for (auto j = libs.begin(); j < libs.end(); j++) {
		dlclose(std::get<0>(*j));
	}
	return 0;
}


