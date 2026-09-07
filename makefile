all:
	@-mkdir bin
	$(CC) test/main.c -DDEBUG -g -o bin/main.bin
	./bin/main.bin
