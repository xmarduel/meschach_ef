TESTS = cex1 cex2 cex3 histdemo

LIB	= ../src/libvopl.a 
LIBS	= -lvogle -lX11

SRC = cex1.c cex2.c cex3.c histdemo.c

OBJS = cex1.o cex2.o cex3.o histdemo.o

MCFLAGS = -g

CFLAGS = -I../src $(MCFLAGS)

all: $(TESTS)

cex1: cex1.o $(LIB)
	cc -o $@ $@.o $(LIB) $(CFLAGS) $(LIBS) -lm

cex2: cex2.o $(LIB)
	cc -o $@ $@.o $(LIB) $(CFLAGS) $(LIBS) -lm

cex3: cex3.o $(LIB)
	cc -o $@ $@.o $(LIB) $(CFLAGS) $(LIBS) -lm

histdemo: histdemo.o $(LIB)
	cc -o $@ $@.o $(LIB) $(CFLAGS) $(LIBS) -lm

clean:
	rm -f *.o core

clobber:
	rm -f $(TESTS) *.o core
