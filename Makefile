CC=g++
CFLAGS=-I.

client: client.cpp requests.cpp helpers.cpp buffer.cpp json.hpp
	$(CC) -o client client.cpp requests.cpp helpers.cpp buffer.cpp json.hpp -Wall -g -std=c++11

run: client
	./client

clean:
	rm -f *.o client
