CC=gcc

target: main.o lab1_IO.o
	gcc main.o lab1_IO.o -o main -lm

serialtester: lab1_IO.o serialtester.o
	gcc serialtester.o lab1_IO.o -o serialtester -lm

matrixgen: 
	gcc matrixgen.c -o matrixgen

main.o:
	gcc -c main.c -o main.o

lab1_IO.o:
	gcc -c lab1_IO.c -o lab1_IO.o 

serialtester.o:
	gcc -c serialtester.c -o serialtester.o

clean:
	rm *.o main serialtester matrixgen