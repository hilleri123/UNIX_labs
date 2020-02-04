CXX=g++
CXXFLAGS= -lnsl 
	  #-lsocket

SERVER_SRC= \
	server.cpp \
	poly.cpp

CLIENT_SRC= \
	client.cpp

HEADERS= \
	poly.h

SERVER= Server
CLIENT= Client

all:$(SERVER) $(CLIENT)

$(SERVER): $(HEADERS) $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $@

$(CLIENT): $(HEADERS) $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $@


