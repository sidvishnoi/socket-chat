CXX=g++
BIN=bin
SRC_SERVER=$(shell find ./src/server -name "*.cc")
SRC_CLIENT=$(shell find ./src/client -name "*.cc")
OBJ_SERVER=$(subst .cc,.o,$(SRC_SERVER))
OBJ_CLIENT=$(subst .cc,.o,$(SRC_CLIENT))

all: server client

server: $(OBJ_SERVER) src/server/server.h
	$(CXX) -o server $(OBJ_SERVER)

client: $(OBJS_CLIENT) src/server/client.h
	$(CXX) -o client $(OBJS_CLIENT)

%.o: %.cc
	$(CXX) -c -o $@ $<

clean:
	rm -rf $(shell find . -name "*.o")
	rm -f ./client
	rm -f ./server
