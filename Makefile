all: server client client_example test

server:server_example.cpp
	g++ -o server server_example.cpp  -lpthread  -g

client:client.cpp
	g++ -o client client.cpp  -lpthread -g

client_example:client_example.cpp
	g++ -o client_example client_example.cpp  -lpthread -g

test:test.cpp
	g++ -o test test.cpp -lpthread -g

clean:
	rm -rf server client test client_example
