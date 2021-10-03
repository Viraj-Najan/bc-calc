all : bc clean
Num.o : Num.c Num.h
	gcc -Wall -c -g Num.c
stack.o : Num.h Num.c stack.h stack.c
	gcc -Wall -c -g stack.c
main.o : infix.c infix.h main.c
	gcc -Wall -c -g main.c
infix.o : stack.c stack.h infix.c infix.h
	gcc -Wall -c -g infix.c
bc : Num.o stack.o main.o infix.o
	gcc -Wall -g Num.o stack.o main.o infix.o -o bc
clean : 
	rm *.o