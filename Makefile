# Victor Forbes - 9293394

ifdef case
	in = < TestCases/$(case).in
endif

all:
	gcc -o main src/*.c -I./includes
debug:
	gcc -o main src/*.c -I./includes -g -Wall
run:
	./main $(in)
fullrun:
	valgrind -v --track-origins=yes --leak-check=full ./main $(in)