#

LIBS = libvopl.a -lvogle -lsuntool -lsunwindow -lpixrect -lm

SRCS =  axis.c adjustscale.c switches.c mem.c spline.c \
	cubicsp.c plot.c err.c hist.c lsfit.c ilinfit.c scalers.c savestr.c

OBJS =  axis.o adjustscale.o switches.o mem.o spline.o \
	cubicsp.o plot.o err.o hist.o lsfit.o ilinfit.o scalers.o savestr.o

FSRCS = sunfort/faxis.c sunfort/flsfit.c sunfort/fplot.c \
	sunfort/fsadjust.c sunfort/fswitch.c

FOBJS = sunfort/faxis.o sunfort/flsfit.o sunfort/fplot.o \
	sunfort/fsadjust.o sunfort/fswitch.o

MCFLAGS = -g -I/tmp

CFLAGS = $(MCFLAGS)

RANLIB = ranlib

all:	libvopl.a 

libvopl.a:	$(OBJS) $(FOBJS)
	rm -f libvopl.a
	ar rcv	libvopl.a $(OBJS) $(FOBJS)
	$(RANLIB) libvopl.a
	
$(OBJS): vopl.h

$(FOBJS): vopl.h
	cd sunfort; make MCFLAGS="$(MCFLAGS)"

clean:
	rm -f *.o sunfort/*.o core

lint:
	lint -I. $(SRCS) $(FSRCS) -lm > vopl.lint

clobber:
	rm -f libvopl.a *.o sunfort/*.o core

