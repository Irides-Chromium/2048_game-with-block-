CC=gcc
CFLAGS=-Wall
LIBS=-lcurses

OBJ=2048.c
OBJ_C=2048_c.c

all: 2048 2048_c

2048: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $<

2048_c: $(OBJ_C)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f 2048 2048_c
