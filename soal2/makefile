CC = gcc
CFLAGS = -Wall -pthread
LDFLAGS = -lrt

all: server client

server: orion.c
	$(CC) $(CFLAGS) orion.c -o orion $(LDFLAGS)

client: eternal.c
	$(CC) $(CFLAGS) eternal.c -o eternal $(LDFLAGS)

clean:
	rm -f orion eternal

clear_ipc:
	ipcrm -M 1234
	ipcrm -Q 5678
