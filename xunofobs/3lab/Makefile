CC= gcc
CFLAGS= -Wall -c

CXX= g++
CXXFLAGS= -Wall



SOURCE= create.o \
	delete.o \
	pull.o \
	push.o \
	status.o

HEADER= function.h

all: test myProject 

SRCS= $(SOURCE:%.o=%.c)

test: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS)



OFILES= $(wildcard *.o)

myProject: main.cpp test
	echo $(OFILES)
	$(CXX) $(CXXFLAGS) main.cpp $(OFILES) -o $@




clean: 
	echo "'clean $(OFILES)'"
	rm -f $(OFILES)

