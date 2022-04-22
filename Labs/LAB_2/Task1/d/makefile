all:T1d

T1d: T1d.o
	gcc -g -m32 -Wall -o T1d T1d.o

T1d.o: T1d.c
	gcc -g -m32 -Wall -c -o T1d.o T1d.c

.PHONY: clean
clean:
	rm -f *.o T1d

