all: client server

client:
	gcc -pthread -g -Wall -o client Asst4_testclient.c
server:
	gcc -pthread -g -Wall -o server Asst4_testserver.c
clean:
	rm -f client
	rm -f server
	rm -rf *.o
