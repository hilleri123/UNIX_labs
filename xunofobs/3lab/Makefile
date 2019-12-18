CC= gcc
CFLAGS= -Wall

CXX= g++
CXXFLAGS= -Wall




all: myProject 



functions.o: functions.c
	$(CC) $(CFLAGS) -c functions.c -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@


myProject: main.o functions.o
	$(CXX) $(CXXFLAGS) main.o functions.o -o $@




