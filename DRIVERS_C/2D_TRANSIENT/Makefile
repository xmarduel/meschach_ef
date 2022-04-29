
SRC = HEAT_2D.c

OBJ = $(SRC:.c=.o)

EXE = $(2D_TRANSIENT_EXE) 

include ../../mk.inc

CFLAGS += -I../.. -I/usr/include -I/Users/xavier/x_local/include -I/Users/xavier/x_local/vogle/src
CFLAGS += -DHAVE_X11 -DHAVE_VOGLE -DHAVE_CPGPLOT -DHAVE_LIBSCIPLOT
#

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) $(MESCHACH_LIBS) $(LIBSCIPLOT_LIBS) $(CPGPLOT_LIBS) $(VOGLE_VOPL_LIBS) $(X11_LIBS) $(MISC_LIBS) $(PTHREAD_LIBS) -lm -o $(EXE)
clean:
	-$(RM) $(OBJ)
	-$(RM) $(EXE)

depend:;


