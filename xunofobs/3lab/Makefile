CC= gcc
CFLAGS= -Wall

CXX= g++
CXXFLAGS= -Wall




all: myProject 



functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c -o $@

main.o: main.cpp functions.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@


myProject: main.o functions.o
	$(CXX) $(CXXFLAGS) main.o functions.o -o $@




