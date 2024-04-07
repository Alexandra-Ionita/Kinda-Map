#Ionita Alexandra-Mihaela 314CB

CC = gcc
CFLAGS = -Wall -g

build: tema3

tema3: tema3.c main.c
	$(CC) $(CFLAGS) $^ -o $@

run:
	./tema3

.PHONY: clean
clean:
	rm -f *.o *~ tema3
