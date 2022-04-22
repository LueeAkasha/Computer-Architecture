all:mydigitcounter

mydigitcounter: mydigitcounter.o
	gcc -g -m32 -Wall -fno-pie -o mydigitcounter mydigitcounter.o

mydigitcounter.o: mydigitcounter.c
	gcc -g -m32 -Wall -fno-pie -c -o mydigitcounter.o mydigitcounter.c

.PHONY: clean
clean:
	rm -f *.o mydigitcounter
