# change application name here (executable output name)
TARGET=IOProject
 
# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall
 
PTHREAD=-pthread
 
CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe
 
GTKLIB=`pkg-config --cflags --libs gtk+-3.0`
 
# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic
 
OBJS=    main.o
 
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
	$(CC) src/main.c -o $(TARGET) $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o pending src/pending.c $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o floyd src/floyd.c $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o drawing src/drawing.c -lm $(WARN) $(GTKLIB) -export-dynamic

main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o

pending.o: src/pending.c
	$(CC) -c $(CCFLAGS) src/pending.c $(GTKLIB) -o pending.o
    
clean:
	rm -f *.o $(TARGET)
