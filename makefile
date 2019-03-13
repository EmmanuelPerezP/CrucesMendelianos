# change application name here (executable output name)
TARGET=bin/cruces
 
# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=
 
PTHREAD=-pthread
 
CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe
 
GTKLIB=`pkg-config --cflags --libs gtk+-3.0`
 
# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic
 
OBJS=    main.o
 
all:
	$(CC) src/cruces.c -o $(TARGET) $(WARN) $(CCFLAGS) $(GTKLIB) -export-dynamic

main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o

pending.o: src/pending.c
	$(CC) -c $(CCFLAGS) src/pending.c $(GTKLIB) -o pending.o
    
clean:
	rm -f *.o $(TARGET)
