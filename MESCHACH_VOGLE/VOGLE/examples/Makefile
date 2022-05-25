#
# examples makefile 
#
CEXAMPS = trivial simple text shapes poly views slant circtxt moretxt jtext curves \
	  windows lstyles getstr balls objvws world loc teapot lotsofwindows \
	  cube lcube tetra licosa beer picking pick

COBJS = trivial.o simple.o text.o shapes.o poly.o views.o slant circtxt.o jtext.o moretxt.o \
	windows.o lstyles.o getstr.o curves.o balls.o objvws.o world.o loc.o \
	teapot.o cube.o lcube.o tetra.o licosa.o beer.o picking.o pick.o lotsofwindows.o

#	patches.o 
#
# Where to find library
#LIB = ../src/libvogle.a
LIB = /Users/xavier/x_local/lib/libvogle-2.0.dylib
LIBS = -L/opt/X11/lib  -lX11 -lm
#
MCFLAGS = -g
CFLAGS = -I../src $(MCFLAGS)

all:	bug $(CEXAMPS)

bug: bug.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

lotsofwindows: lotsofwindows.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)


trivial: trivial.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

simple: simple.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

text: text.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

shapes: shapes.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

poly: poly.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

views: views.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

circtxt: circtxt.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

slant: slant.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

moretxt: moretxt.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

jtext: jtext.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

curves: curves.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

lstyles: lstyles.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

windows: windows.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

getstr: getstr.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

patches: patches.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

balls: balls.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

objvws: objvws.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

world: world.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

loc: loc.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

teapot: teapot.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

cube: cube.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

lcube: lcube.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

tetra: tetra.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

licosa: licosa.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

beer: beer.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

picking: picking.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

pick: pick.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB) $(LIBS)

clean:
	rm -f *.o core

clobber:
	rm -f $(CEXAMPS) bug *.o core
