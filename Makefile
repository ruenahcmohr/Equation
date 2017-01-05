
CC=gcc
CFLAGS=-c 


all:   main.o eq5.o eqll.o ops.o varlib.o
	$(CC) main.o eq5.o eqll.o ops.o varlib.o -o eq -lm

main.o: main.c returnValues.h
	$(CC) $(CFLAGS)  main.c

eq5.o: eq5.c eq5.h
	$(CC) $(CFLAGS) eq5.c

returnValues.o: returnValues.c returnValues.h 
	$(CC) $(CFLAGS)  returnValues.c
        
eqll.o: eqll.c eqll.h returnValues.h
	$(CC) $(CFLAGS) eqll.c 

ops.o: ops.c ops.h returnValues.h
	$(CC) $(CFLAGS) ops.c 

varlib.o: varlib.c varlib.h returnValues.h
	$(CC) $(CFLAGS) varlib.c                 
      
clean:
	rm *.o eq

run: all
	./eq

