target: main.o lab1_IO.o
	gcc -std=c99 -pthread main.o lab1_IO.o -o main -lm

serialtester: lab1_IO.o serialtester.o
	gcc -std=c99 serialtester.o lab1_IO.o -o serialtester -lm

matrixgen: 
	gcc matrixgen.c -o matrixgen

main.o:
	gcc -std=c99 -c main.c -o main.o

lab1_IO.o:
	gcc -std=c99 -c lab1_IO.c -o lab1_IO.o 

serialtester.o:
	gcc -std=c99 -c serialtester.c -o serialtester.o

clean:
	rm *.o main serialtester matrixgen