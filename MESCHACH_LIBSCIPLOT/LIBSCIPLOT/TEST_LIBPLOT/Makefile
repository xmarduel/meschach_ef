DRAW_C = draw_c
GUI = gui

CFLAGS = -DLIBPLOT  -I.. -I/Users/xavier/x_local/plotutils-2.6 -I/Users/xavier/x_local/plotutils-2.6/include -I/opt/X11/include
CFLAGS_MESCHACH = -I../../..

LDFLAGS = -L/Users/xavier/x_local/lib  -L/opt/X11/lib  -lplot  -lXaw -lXt -lX11
LDFLAGS_MESCHACH = -L/Users/xavier/x_local/lib  -lmeschach_adds -lmeschach

$(DRAW_C).o: $(DRAW_C).c
	$(CC) $(CFLAGS) -c $(DRAW_C).c
$(DRAW_C): $(DRAW_C).o
	$(CC) -o $(DRAW_C) $(LDFLAGS) $(DRAW_C).o

$(GUI).o: $(GUI).c
	$(CC) $(CFLAGS) -c $(GUI).c
$(GUI): $(GUI).o
	$(CC) -o $(GUI) $(LDFLAGS) $(GUI).o

all: $(DRAW_C) $(GUI)

clean::
	rm -f *.o $(DRAW_C) $(GUI)
