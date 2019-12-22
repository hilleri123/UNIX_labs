CXX= g++
CXXFLAGS= -Wall -lrt


targets: Comp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

Comp: main.o
	$(CXX) $(CXXFLAGS) main.o -o $@




