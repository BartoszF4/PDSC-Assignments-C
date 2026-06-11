CC = gcc
CFLAGS = -Wall -pedantic -g
TARGET = bsearch
OBJS = main.o binarySearch.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c binarySearch.h
	$(CC) $(CFLAGS) -c main.c

binarySearch.o: binarySearch.c binarySearch.h
	$(CC) $(CFLAGS) -c binarySearch.c

clean:
	rm -f *.o $(TARGET)