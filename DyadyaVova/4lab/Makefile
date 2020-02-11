CC= gcc
CFLAGS= -Wall

CXX= g++
CXXFLAGS= -Wall




all: myProject 




main.o: main.cpp functions.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@


myProject: main.o 
	$(CXX) $(CXXFLAGS) main.o -o $@




