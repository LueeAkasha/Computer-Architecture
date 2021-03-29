all: main

main: mainAssignment0.o asmAssignment0.o
	gcc -g -m32 -Wall -o main mainAssignment0.o asmAssignment0.o

mainAssignment0.o: mainAssignment0.c
	gcc -g -m32 -Wall -c -o mainAssignment0.o mainAssignment0.c

asmAssignment0.o: asmAssignment0.s
	nasm -g -f elf -w+all -o asmAssignment0.o asmAssignment0.s

.PHONY: clean

clean:
	rm -f *.o main
