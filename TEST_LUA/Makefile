LUA=/Users/xavier/x_local/lua-5.4.3/src
#LIBS=-lm -ldl
LIBS=

SRCS= test1.c test2.c
OBJS= test1.o test2.o
PROGS= test1 test2

all: $(OBJS)
	gcc -o test1 -L$(LUA) test1.o -llua $(LIBS)
	gcc -o test2 -L$(LUA) test2.o -llua $(LIBS)

$(OBJS): $(SRCS)
	gcc -c -I$(LUA) test1.c
	gcc -c -I$(LUA) test2.c

clean:
	rm -fr $(OBJS) $(PROGS)
