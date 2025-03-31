#
# examples makefile 
#
PEXAMPS = ptrivial psimple pshapes ppoly pviews pcirctxt pmoretxt pcurves \
	  pgetstr ppatches pballs pobjvws pworld ploc ptetra pcube \
	  plcube pjtext plstyles

POBJS = ptrivial.o psimple.o pshapes.o ppoly.o pviews.o pcirctxt.o pmoretxt.o \
	pgetstr.o pcurves.o ppatches.o pballs.o pobjvws.o pworld.o ploc.o \
	ptetra.o pcube.o plcube.o pjtext.o plstyles.o

#
# Where to find library
LIB = ../../src/libvogle.a
LIBS = -lX11
#
PAS = pc

#
# The -I is so we can include Vogle.h
#
MPFLAGS = 
PFLAGS = $(MPFLAGS) -I../../src/sunpas

.p.o:
	$(PAS) -c $(PFLAGS) $*.p

all:	$(PEXAMPS)

ptrivial: ptrivial.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

psimple: psimple.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

pshapes: pshapes.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

ppoly: ppoly.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

pviews: pviews.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

pcirctxt: pcirctxt.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

pmoretxt: pmoretxt.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

pcurves: pcurves.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

pgetstr: pgetstr.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

ppatches: ppatches.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

pballs: pballs.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

pobjvws: pobjvws.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

pworld: pworld.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

ploc: ploc.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

ptetra: ptetra.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

pcube: pcube.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

plcube: plcube.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

pjtext: pjtext.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

plstyles: plstyles.o $(LIB)
	$(PAS) $(PFLAGS) -o $@ $@.o $(LIB) $(LIBS) 

clean:
	rm -f *.o core

clobber:
	rm -f $(PEXAMPS) *.o core
