
CFLAGS = -I/opt/X11/include -I../../src $(MCFLAGS)

EX = xtballs xtlcube

all: $(EX)

xtballs: xtballs.o balls.o
	$(CC) $(CFLAGS) -o xtballs xtballs.o balls.o /Users/xavier/x_local/lib/libvogle-2.0.dylib -L/opt/X11/lib -lXt -lXaw -lXmu -lXext -lX11 -lm

xtlcube: xtlcube.o lcube.o
	$(CC) $(CFLAGS) -o xtlcube xtlcube.o lcube.o /Users/xavier/x_local/lib/libvogle-2.0.dylib -L/opt/X11/lib -lXt -lXaw -lXmu -lXext -lX11 -lm

clean:
	rm -f $(EX) *.o core

clobber:
	rm -f $(EX) *.o core
