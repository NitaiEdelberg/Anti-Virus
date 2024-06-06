all: BubbleSort antivirus valgrid

BubbleSort: BubbleSort.o

	gcc -g -m32 -Wall -o BubbleSort BubbleSort.o

BubbleSort.o: BubbleSort.c

	gcc -m32 -g -Wall -c -o BubbleSort.o BubbleSort.c

antivirus: antivirus.o

	gcc -g -m32 -Wall -o antivirus antivirus.o

antivirus.o: antivirus.c

	gcc -m32 -g -Wall -c -o antivirus.o antivirus.c

valgrid:

	valgrind --leak-check=full --show-reachable=yes BubbleSort 3 4 2 1

.PHONY: clean

clean:

	rm -f *.o BubbleSort antivirus