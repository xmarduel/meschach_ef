
CFLAGS = -I../../src $(MCFLAGS)

EX = xtballs xtlcube

all: $(EX)

xtballs: xtballs.o balls.o
	$(CC) $(CFLAGS) -o xtballs xtballs.o balls.o ../../src//libvogle.a -lXt -lXaw -lXmu -lXext -lX11 -lm

xtlcube: xtlcube.o lcube.o
	$(CC) $(CFLAGS) -o xtlcube xtlcube.o lcube.o ../../src//libvogle.a -lXt -lXaw -lXmu -lXext -lX11 -lm

clean:
	rm -f $(EX) *.o core

clobber:
	rm -f $(EX) *.o core
