#
# examples makefile 
#
FEXAMPS = ftrivial fsimple fshapes fpoly fviews fcirctxt fmoretxt fcurves \
	  fjtext fgetstr fpatches fballs fobjvws fworld floc ftetra fcube \
	  flcube flstyles

FOBJS = ftrivial.o fsimple.o fshapes.o fpoly.o fviews.o fcirctxt.o fmoretxt.o \
	fjtext.o fgetstr.o fcurves.o fpatches.o fballs.o fobjvws.o fworld.o floc.o \
	ftetra.o fcube.o flcube.o flstyles.o

#
# Where to find library
LIB = ../../src/libvogle.a
LIBS = -lX11
#
F77 = f77

MFFLAGS = -g
FFLAGS = $(MFFLAGS) -C

.f.o:
	$(F77) -c $(FFLAGS) $*.f

all:	$(FEXAMPS)

ftrivial: ftrivial.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fsimple: fsimple.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fshapes: fshapes.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fpoly: fpoly.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fviews: fviews.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fcirctxt: fcirctxt.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fmoretxt: fmoretxt.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fcurves: fcurves.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fjtext: fjtext.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fgetstr: fgetstr.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fpatches: fpatches.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fballs: fballs.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fobjvws: fobjvws.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fworld: fworld.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

floc: floc.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

ftetra: ftetra.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

fcube: fcube.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

flcube: flcube.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

flstyles: flstyles.o $(LIB)
	$(F77) $(FFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

clean:
	rm -f *.o core

clobber:
	rm -f $(FEXAMPS) *.o core
