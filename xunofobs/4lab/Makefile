CXX=g++
CXXFLAGS=

SERVER_SRC= \
	server.cpp \
	sender_svc.c \
	sender_xdr.c 

CLIENT_SRC= \
	client.c \
	sender_clnt.c \
	sender_xdr.c 

HEADERS= \
	sender.h 

SERVER= Server
CLIENT= Client

all:$(SERVER) $(CLIENT)

$(SERVER): $(HEADERS) $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $@

$(CLIENT): $(HEADERS) $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $@


