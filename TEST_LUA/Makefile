SRCS= test1.c test2.c
OBJS= test1.o test2.o
PROGS= test1 test2

all: $(OBJS)
	gcc -o test1 -L./lua-5.3.6/src test1.o -llua -lm -ldl
	gcc -o test2 -L./lua-5.3.6/src test2.o -llua -lm -ldl

$(OBJS): $(SRCS)
	gcc -c -I./lua-5.3.6/src test1.c
	gcc -c -I./lua-5.3.6/src test2.c

clean:
	rm -fr $(OBJS) $(PROGS)
