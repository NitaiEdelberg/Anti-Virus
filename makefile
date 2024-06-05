all: BubbleSort valgrid

BubbleSort: BubbleSort.o

	gcc -g -m32 -Wall -o BubbleSort BubbleSort.o

BubbleSort.o: BubbleSort.c

	gcc -m32 -g -Wall -c -o BubbleSort.o BubbleSort.c

valgrid:

	valgrid --leack-check=full --show-reachable=yes BubbleSort "3 4 2 1"

.PHONY: clean

clean:

	rm -f *.o BubbleSort