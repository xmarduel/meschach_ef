#
# Makefile for sunpascal pc interface routines.
#

MCFLAGS = -g -fsingle -f68881 /usr/lib/libm.il

SRC =	parcs.c paspect.c pattr.c pbuffer.c pcurves.c pclip.c pdraw.c \
	pdrivers.c pgetgp.c pviewing.c pviewp.c pmatrix.c pmove.c  \
	pobjects.c ppatches.c ppoints.c ppoly.c prect.c ptrans.c  \
	pyobbaray.c ptext.c pgetstr.c ppref.c

OBJS = $(SRC:.c=.o)

CFLAGS = -I../ $(MCFLAGS)

all:	$(OBJS)

$(OBJS): ../vogle.h

clean:
	rm -f $(OBJS) core

