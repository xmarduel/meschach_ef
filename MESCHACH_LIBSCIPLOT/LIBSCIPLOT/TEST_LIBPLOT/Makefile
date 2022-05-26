DRAW_C = draw_c
GUI = gui
ANIM = anim
LETTER = letter

CFLAGS = -DLIBPLOT  -I.. -I$(MESCHACH_EF_PROJECT)/EXTERNAL_LIBS/plotutils-2.6 -I$(MESCHACH_EF_PROJECT)/EXTERNAL_LIBS/plotutils-2.6/include -I/opt/X11/include
CFLAGS_MESCHACH = -I../../..

LDFLAGS = -L/Users/xavier/x_local/lib  -L/opt/X11/lib  -lplot  -lXaw -lXt -lX11
LDFLAGS_MESCHACH = -L/Users/xavier/x_local/lib  -lmeschach_adds -lmeschach

$(DRAW_C): $(DRAW_C).o
	$(CC) -o $(DRAW_C) $(LDFLAGS) $(DRAW_C).o
$(DRAW_C).o: $(DRAW_C).c
	$(CC) $(CFLAGS) -c $(DRAW_C).c


$(GUI): $(GUI).o
	$(CC) -o $(GUI) $(LDFLAGS) $(GUI).o
$(GUI).o: $(GUI).c
	$(CC) $(CFLAGS) -c $(GUI).c

$(ANIM): $(ANIM).o
	$(CC) -o $(ANIM) $(LDFLAGS) $(ANIM).o
$(ANIM).o: $(ANIM).c
	$(CC) $(CFLAGS) -c $(ANIM).c

$(LETTER): $(LETTER).o
	$(CC) -o $(LETTER) $(LDFLAGS) $(LETTER).o
$(LETTER).o: $(LETTER).c
	$(CC) $(CFLAGS) -c $(LETTER).c

all: $(DRAW_C) $(GUI) $(ANIM) $(LETTER)

clean::
	rm -f *.o $(DRAW_C) $(GUI) $(ANIM) $(LETTER)
