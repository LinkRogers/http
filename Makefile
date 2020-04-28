all:
	g++ -o server server_example.cpp  -lpthread 
	g++ -o client client_example.cpp  -lpthread 
	g++ -o test_client test.cpp -lpthread
clean:
	rm -rf server client test_client
