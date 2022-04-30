FILE = toto
#FILE = anim
#FILE = letter
#FILE = gui

SRC = $(FILE).c
OBJ = $(FILE).o

EXE = $(FILE)

CFLAGS = -I/opt/X11/include -I/Users/xavier/x_local/plotutils-2.6/include
LIBS = -lplot -lXaw -lXmu -lXt -lSM -lICE -lXext -lX11
LDFLAGS = \
	-L/Users/xavier/x_local/plotutils-2.6/libplot/.libs \
	-L/Users/xavier/x_local/plotutils-2.6/lib \
	-L/usr/X11/lib

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c  $(SRC)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o $(EXE)

all:: $(EXE)
clean:
	rm -f $(OBJ) $(EXE)
