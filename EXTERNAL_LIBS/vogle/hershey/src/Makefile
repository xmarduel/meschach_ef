#
# Makes the binary Hershey font file for VOGLE
#
LIB = ../../src/libvogle.a
CC = cc
CFLAGS = -I../../src $(MCFLAGS)
#LIBS = -lsuntool -lsunwindow -lpixrect  -lm
SHELL = /bin/sh

.IGNORE:

HDR = h2v.h

all:	h2v hdisp fdisp fonts

h2v:	h2v.o getchar.o
	$(CC) $(CFLAGS) -o $@ $@.o getchar.o

hdisp:	hdisp.o getchar.o ../../src/libvogle.a
	$(CC) $(CFLAGS) -o $@ $@.o getchar.o $(LIB) $(LIBS)

fdisp:	fdisp.o ../../src/libvogle.a
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

$(FONTLIB):
	if test ! -d $(FONTLIB);\
	then mkdir $(FONTLIB);\
	fi;

fonts:	h2v $(FONTLIB)
	./h2v ../data/hersh.oc
	./h2v ../data/hersh.or ../fonts/japan.hmp japanese
	mv astrology cursive cyrillic futura.l futura.m gothic.eng \
	gothic.ger gothic.ita greek japanese markers math.low \
	math.upp meteorology music script symbolic times.g \
	times.i times.ib times.r times.rb $(FONTLIB)
	chmod a+r $(FONTLIB)/*
	touch fonts

h2v: h2v.h

clean:
	rm -f astrology cursive cyrillic futura.l futura.m gothic.eng \
	gothic.ger gothic.ita greek japanese markers math.low \
	math.upp meteorology music script symbolic times.g \
	times.i times.ib times.r times.rb *.o core fonts

clobber:
	rm -f astrology cursive cyrillic futura.l futura.m gothic.eng \
	gothic.ger gothic.ita greek japanese markers math.low \
	math.upp meteorology music script symbolic times.g \
	times.i times.ib times.r times.rb *.o core h2v hdisp fdisp fonts
