CXX=g++
CXXFLAGS= 

SERVER_SRC= \
	server.cpp \
	del_svc.c \
	del_xdr.c \
	poly.cpp

CLIENT_SRC= \
	client.c \
	del_clnt.c \
	del_xdr.c 

HEADERS= \
	del.h \
	poly.h
SERVER= server
CLIENT= client

all:$(SERVER) $(CLIENT)

$(SERVER): $(HEADERS) $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $@

$(CLIENT): $(HEADERS) $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $@


