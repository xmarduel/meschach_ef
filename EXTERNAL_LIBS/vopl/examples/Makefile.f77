TESTS = fex1 fex2 fex3

LIB	= ../src/libvopl.a 
LIBS	= -lvogle -lX11

FSRC = fex1.f fex2.f fex3.f

FOBJS = fex1.o fex2.o fex3.o

MFFLAGS = -g

FFLAGS = $(MFFLAGS)

.f.o:
	$(F77) $(FFLAGS) -c $*.f

all: $(TESTS)

fex1: fex1.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) -lm

fex2: fex2.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) -lm

fex3: fex3.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) -lm

clean:
	rm -f *.o core

clobber:
	rm -f $(TESTS) *.o core
