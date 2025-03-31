#
# if your system is running 4.2 or is a sun running
# a sun os before 4.0 you need to add -DBSD to CFLAGS
#
MCFLAGS = 

HDRS= ../src/vogle.h

DOBJS = X11.o ps.o hpdxy.o tek.o ppm.o

CFLAGS= -I../src $(MCFLAGS)

all:	$(DOBJS)

$(DOBJS): $(HDRS)

clean:
	rm -f core $(DOBJS)

clobber:
	rm -f core *.o
	
