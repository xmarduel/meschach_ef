TEST1_LIBSCIPLOT = test1_libsciplot
TEST2_LIBSCIPLOT = test2_libsciplot

CFLAGS = -I.. -I/Users/xavier/x_local/plotutils-2.6 -I/Users/xavier/x_local/plotutils-2.6/include
CFLAGS_MESCHACH = -I../../..

LDFLAGS = -L/Users/xavier/x_local/lib  -L/opt/X11/lib  -lsciplot -lplot  -lXaw -lXt -lX11
LDFLAGS_MESCHACH = -L/Users/xavier/x_local/lib  -lmeschach_adds -lmeschach

$(TEST1_LIBSCIPLOT).o: $(TEST1_LIBSCIPLOT).c
	$(CC) $(CFLAGS) -c $(TEST1_LIBSCIPLOT).c
$(TEST1_LIBSCIPLOT): $(TEST1_LIBSCIPLOT).o
	$(CC) -o $(TEST1_LIBSCIPLOT) $(LDFLAGS) $(TEST1_LIBSCIPLOT).o

$(TEST2_LIBSCIPLOT).o: $(TEST2_LIBSCIPLOT).c
	$(CC) $(CFLAGS) -c $(TEST2_LIBSCIPLOT).c
$(TEST2_LIBSCIPLOT): $(TEST2_LIBSCIPLOT).o
	$(CC) -o $(TEST2_LIBSCIPLOT) $(LDFLAGS) $(TEST2_LIBSCIPLOT).o

all: $(TEST1_LIBSCIPLOT) $(TEST2_LIBSCIPLOT)

clean::
	rm -f *.o $(TEST1_LIBSCIPLOT) $(TEST2_LIBSCIPLOT)
