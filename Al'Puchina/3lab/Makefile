CC= gcc
CFLAGS= -Wall -lrt

CXX= g++
CXXFLAGS= -Wall -lrt


targets: Client Server


functions.o: functions.h functions.c
	$(CC) $(CFLAGS) -c functions.c -o $@


client.o: functions.h client.cpp conf.h
	$(CXX) $(CXXFLAGS) -c client.cpp -o $@

server.o: functions.h server.cpp conf.h
	$(CXX) $(CXXFLAGS) -c server.cpp -o $@


Client: client.o functions.o functions.h conf.h
	$(CXX) $(CXXFLAGS) client.o functions.o -o $@

Server: server.o functions.o functions.h conf.h
	$(CXX) $(CXXFLAGS) server.o functions.o -o $@




