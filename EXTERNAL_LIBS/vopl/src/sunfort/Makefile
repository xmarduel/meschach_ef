#

SRCS = faxis.c flsfit.c fplot.c fhist.c fsadjust.c fswitch.c

OBJS = faxis.o flsfit.o fplot.o fhist.o fsadjust.o fswitch.o

MCFLAGS = -fsingle -f68881 -g

CFLAGS = $(MCFLAGS) -I..

all:	$(OBJS)

$(OBJS): ../vopl.h

clean:
	rm -f *.o core

