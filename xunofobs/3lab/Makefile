CC= gcc
CFLAGS= -Wall -c

CXX= g++
CXXFLAGS= -Wall



OBJ= create.o \
	delete.o \
	pull.o \
	push.o \
	status.o

HEADER= function.h

all: test myProject 

SRCS= $(OBJ:%.o=%.c)

test: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS)




myProject: main.cpp $(test) $(OBJ)
	$(CXX) $(CXXFLAGS) main.cpp $(OBJ) -o $@




clean: 
	echo "'clean $(OFILES)'"
	rm -f $(OFILES)

