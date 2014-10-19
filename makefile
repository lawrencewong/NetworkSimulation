CC = gcc
run: build
	./n7&
	./n6&
	./n5&
	./n4&
	./n3&
	./n2&
	./n1

build: n1 n2 n3 n4 n5 n6 n7

n1: n1.c
	$(CC) n1.c -o n1

n2: n2.c
	$(CC) n2.c -o n2

n3: n3.c
	$(CC) n3.c -o n3

n4: n4.c
	$(CC) n4.c -o n4

n5: n5.c
	$(CC) n5.c -o n5

n6: n6.c
	$(CC) n6.c -o n6

n7: n7.c
	$(CC) n7.c -o n7
