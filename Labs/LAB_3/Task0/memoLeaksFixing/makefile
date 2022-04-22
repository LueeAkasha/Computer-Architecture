all: lab3_bubblesort

lab3_bubblesort: lab3_bubblesort.o 
	gcc -g -m32 -Wall -o lab3_bubblesort lab3_bubblesort.o

lab3_bubblesort.o: lab3_bubblesort.c
	gcc -g -m32 -Wall -c -o lab3_bubblesort.o lab3_bubblesort.c

.PHONY: clean

clean:
	rm -f *.o lab3_bubblesort
