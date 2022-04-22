all:addresses[1]

addresses[1]: addresses[1].o
	gcc -g -m32 -Wall -no-pie -o addresses[1] addresses[1].o

addresses[1].o: addresses[1].c
	gcc -g -m32 -Wall -c -no-pie -o addresses[1].o addresses[1].c

.PHONY: clean
clean:
	rm -f *.o addresses[1]
