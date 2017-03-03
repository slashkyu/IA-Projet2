.PHONY = all, clean


.SUFFIXES:

CC=gcc
CFLAGS=-Wall -g


LIBS += -lpthread



INCLDIR = -I.

PROGS = classification creation 

all: $(PROGS)

classification.o: classification.c 
	$(CC) $(CFLAGS) $(INCLDIR) -c -o $@ $<

classification: classification.o 
	$(CC) -o $@ $(LDFLAGS) $^ -lm

Creabase_test.o: Creabase_test.c 
	$(CC) $(CFLAGS) $(INCLDIR) -c -o $@ $<

creation: Creabase_test.o 
	$(CC) -o $@ $(LDFLAGS) $^ -lm

clean:
	rm -f $(PROGS) *.o
