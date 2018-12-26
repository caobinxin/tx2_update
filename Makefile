all:
	gcc -o server server.c upload_client.c util.c version_client.c

run:
	./server
