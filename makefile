COMPILER = gcc
CCFLAGS  = -ansi -pedantic -Wall

all: dataConverter  

debug:
	make DEBUG=TRUE

dataConverter: dataConverter.o
	$(COMPILER) $(CCFLAGS) -o dataConverter dataConverter.o
	
dataConverter.o: dataConverter.c dataConverter.h
	$(COMPILER) $(CCFLAGS) -c dataConverter.c dataConverter.h

ifeq ($(DEBUG), TRUE)
 CCFLAGS += -g
endif

clean:
	rm -f dataConverter
	rm -f *.o
	rm -f *~
