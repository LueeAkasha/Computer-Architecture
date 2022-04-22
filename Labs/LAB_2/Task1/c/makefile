all:T1c

T1c: T1c.o
	gcc -g -m32 -Wall -o T1c T1c.o

T1c.o: T1c.c
	gcc -g -m32 -Wall -c -o T1c.o T1c.c

.PHONY: clean
clean:
	rm -f *.o T1c

