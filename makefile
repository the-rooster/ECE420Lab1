CC=gcc

target: main.o lab1_IO.o
	gcc main.o lab1_IO.o -o main

main.o:
	gcc -c main.c -o main.o

lab1_IO.o:
	gcc -c lab1_IO.c -o lab1_IO.o 

clean:
	rm *.o main