CC=gcc
CFLAGS=-g -pthread 
DEPS= procon.h
OBJ = procon.o

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@ 

procon: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm procon *.o
