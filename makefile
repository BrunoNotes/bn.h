all:
	@-mkdir bin
	$(CC) test/main.c -DDEBUG -g -fsanitize=address -o bin/main.bin
	./bin/main.bin
