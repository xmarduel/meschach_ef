#
# Makefile for vopl
#
# Usage: make os
#
# You might have to set:
#
#	OS
#	RANLIB
#	MCFLAGS
#	MCFLAGS
#	LIBS
#	F77
#	MFFLAGS
#
############################################################
#
# You can set -DBSD or -DSYS5 here
#
#OS = -DBSD
#OS = -DSYS5

# If you set -DSYS5 then set RANLIB = "ar ts" or "echo"
RANLIB = ranlib

#
# Global CFLAGS can be set here. These are the ones we use on a sun.
#
# NOTE: change the -L bit or put libvogle.a 
# where the linker can find them....
WHERELIBS = -L/tmp -I/tmp
#

# Specify the default VOGLE device for gpp here.
#
DEFDEV = -DX11

MCFLAGS = -g $(OS) $(WHERELIBS)
#MCFLAGS = $(OS) -O $(WHERELIBS)
#MCFLAGS = $(OS) -O4 -fsingle -f68881 /usr/lib/libm.il
#
# Or on an apollo... (-Wp make it use the 'other cpp')
#MCFLAGS = $(OS) -O -Wp -M3000

#
# If we have fortran we have to say so.
#
F77 = xlf -qextname
# Global FFLAGS can be set here. These are the ones we use on a sun.
#
MFFLAGS = $(OS) -g $(WHERELIBS)
#MFFLAGS = $(OS) -O 

#
# The name of the library to install and where to put it.
#
LIB = libvopl.a
DEST = /usr/lib

#
# Any other libraries that are needed go here
#

SUNLIBS = -lsuntool -lsunwindow -lpixrect -lX11
X11LIBS = -L/usr/X11/lib -lX11
UNIXLIBS = 
APOLLOLIBS =

LIBS = -lvogle $(X11LIBS)


all:
	cd src; make -f Makefile \
			RANLIB="$(RANLIB)" \
			MCFLAGS="$(MCFLAGS)"

	if test -n "$(F77)" ; \
	then 	cd examples; \
		make -f Makefile.f77 \
			MFFLAGS="$(MFFLAGS)" \
			F77="$(F77)" \
			LIBS="$(LIBS)"; \
	fi; exit 0

	cd examples; make -f Makefile \
			MCFLAGS="$(MCFLAGS)" \
			LIBS="$(LIBS)"

	cd gpp; make -f Makefile \
			MCFLAGS="$(MCFLAGS)" \
			DEFDEV="$(DEFDEV)" \
			LIBS="$(LIBS)"

clean:
	cd src; make clean
	cd examples; make clean
	cd examples; make -f Makefile.f77 clean
	cd gpp; make clean

clobber:
	cd src; make clobber
	cd examples; make clobber
	cd examples; make -f Makefile.f77 clobber
	cd gpp; make clobber

install:
	cp src/$(LIB) $(DEST)
	chmod 644 $(DEST)/$(LIB)
	$(RANLIB) $(DEST)/$(LIB)
