CXX=g++
CFLAGS=-std=c++14 -Wall -Wextra
BIN=bin
SRC_SERVER=$(shell find ./src/server ./src/db -name "*.cc")
SRC_CLIENT=$(shell find ./src/client -name "*.cc")
OBJ_SERVER=$(subst .cc,.o,$(SRC_SERVER))
OBJ_CLIENT=$(subst .cc,.o,$(SRC_CLIENT))

all: server client

server: $(OBJ_SERVER) src/server/server.h src/db/database.h
	$(CXX) $(CFLAGS) -c -o src/server/main.o src/server/main.cc
	$(CXX) $(CFLAGS) -o server $(OBJ_SERVER)

client: $(OBJ_CLIENT) src/client/client.h
	$(CXX) $(CFLAGS) -o client $(OBJ_CLIENT)

%.o: %.cc
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(shell find . -name "*.o")
	rm -f ./client
	rm -f ./server
