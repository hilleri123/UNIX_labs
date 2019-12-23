CXX= g++
CXXFLAGS= -Wall -lrt




all: Client Server



controller.o: controller.cpp conf.h
	$(CXX) $(CXXFLAGS) -c controller.cpp -o $@

Server: controller.o 
	$(CXX) $(CXXFLAGS) controller.o -o $@


main.o: main.cpp conf.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@


Client: main.o 
	$(CXX) $(CXXFLAGS) main.o -o $@




