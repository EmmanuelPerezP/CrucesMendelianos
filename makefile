# change application name here (executable output name)
TARGET=bin/IOProject
 
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
	$(CC) -o bin/pending src/pending.c $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o bin/cruces src/cruces.c $(WARN) $(GTKLIB) -export-dynamic
	# $(CC) -o ana src/utility/ana.c $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o bin/drawing src/drawing.c -lm $(WARN) $(GTKLIB) -export-dynamic

main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o

pending.o: src/pending.c
	$(CC) -c $(CCFLAGS) src/pending.c $(GTKLIB) -o pending.o
    
clean:
	rm -f *.o $(TARGET)
