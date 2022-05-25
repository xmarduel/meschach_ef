c
c* demonstrate still more features of text
c
	program fjtext
	integer BLACK, RED, GREEN, BLUE, YELLOW, MAGENTA, WHITE
	integer CYAN
	parameter(BLACK = 0)
	parameter(RED = 1)
	parameter(GREEN = 2)
	parameter(YELLOW = 3)
	parameter(BLUE = 4)
	parameter(MAGENTA = 5)
	parameter(CYAN = 6)
	parameter(WHITE = 7)

	character *20 dev

	write(*,*)'Enter device: '
	read(*, '(a)') dev
	call vinit(dev)

	call textsize(0.03, 0.04)

	call ortho2(0.0, 1.0, 0.0, 1.0)

	call color(RED)
	call clear

	call drawstuff

c
c	Now do it all with the text rotated .... 
c
	call textang(45.0)
	call drawstuff

	call textang(160.0)
	call drawstuff

	call textang(270.0)
	call drawstuff

c
c	Now with a single character 
c

	call textjustify(0)

	call drawstuff2(0.0)

	call drawstuff2(90.0)

	call drawstuff2(160.0)

	call drawstuff2(270.0)

	call vexit
	end

	subroutine drawstuff
	integer BLACK, RED, GREEN, BLUE, YELLOW, MAGENTA, WHITE
	parameter(BLACK = 0)
	parameter(RED = 1)
	parameter(GREEN = 2)
	parameter(YELLOW = 3)
	parameter(BLUE = 4)
	parameter(MAGENTA = 5)
	parameter(WHITE = 7)

	call color(BLACK)
	call polyfill(1)
c		So rect clears a bit for us

	call rect(0.1, 0.1, 0.9, 0.9)
	call color(WHITE)
	call move2(0.1, 0.5)
	call draw2(0.9, 0.5)
	call move2(0.5, 0.1)
	call draw2(0.5, 0.9)

	call color(GREEN)
	call move2(0.5, 0.5)
	call leftjustify
	call drawstr('This is Left Justified text')

	call getkey

	call color(BLACK)
	call rect(0.1, 0.1, 0.9, 0.9)
	call color(WHITE)
	call move2(0.1, 0.5)
	call draw2(0.9, 0.5)
	call move2(0.5, 0.1)
	call draw2(0.5, 0.9)

	call color(YELLOW)
	call move2(0.5, 0.5)
	call centertext(1)
	call drawstr('This is Centered text')
	call centertext(0)

	call getkey

	call color(BLACK)
	call rect(0.1, 0.1, 0.9, 0.9)
	call color(WHITE)
	call move2(0.1, 0.5)
	call draw2(0.9, 0.5)
	call move2(0.5, 0.1)
	call draw2(0.5, 0.9)

	call color(MAGENTA)
	call move2(0.5, 0.5)
	call rightjustify
	call drawstr('This is Right Justified text')
	call textjustify(0)

	call getkey
	end

	subroutine drawstuff2(ang)
	integer BLACK, RED, GREEN, BLUE, YELLOW, MAGENTA, WHITE
	parameter(BLACK = 0)
	parameter(RED = 1)
	parameter(GREEN = 2)
	parameter(YELLOW = 3)
	parameter(BLUE = 4)
	parameter(MAGENTA = 5)
	parameter(WHITE = 7)

	call color(BLACK)
	call rect(0.1, 0.1, 0.9, 0.9)
	call color(WHITE)
	call move2(0.1, 0.5)
	call draw2(0.9, 0.5)
	call move2(0.5, 0.1)
	call draw2(0.5, 0.9)


	call textang(ang)
	call color(GREEN)
	call move2(0.5, 0.5)
	call leftjustify
	call drawchar('B')

	call textang(0.0)
	call textjustify(0)
	call boxtext(0.1, 0.1, 0.4, 0.02, 
     +   'The ''B'' should be leftjustified')
	call pushattributes
	call getkey

	call color(BLACK)
	call rect(0.1, 0.1, 0.9, 0.9)
	call color(WHITE)
	call move2(0.1, 0.5)
	call draw2(0.9, 0.5)
	call move2(0.5, 0.1)
	call draw2(0.5, 0.9)

	call textang(ang)
	call color(YELLOW)
	call move2(0.5, 0.5)
	call centertext(1)
	call drawchar('B')
	call centertext(0)
	call textang(0.0)
	call textjustify(0)
	call boxtext(0.1, 0.1, 0.4, 0.02, 
     +   'The ''B'' should be centered')
	call pushattributes

	call getkey

	call color(BLACK)
	call rect(0.1, 0.1, 0.9, 0.9)
	call color(WHITE)
	call move2(0.1, 0.5)
	call draw2(0.9, 0.5)
	call move2(0.5, 0.1)
	call draw2(0.5, 0.9)

	call textang(ang)
	call color(MAGENTA)
	call move2(0.5, 0.5)
	call rightjustify
	call drawchar('B')
	call textang(0.0)
	call textjustify(0)
	call boxtext(0.1, 0.1, 0.4, 0.02, 
     +   'The ''B'' should be rightjustified')
	call pushattributes

	call getkey

	call color(BLACK)
	call rect(0.1, 0.1, 0.9, 0.9)
	call color(WHITE)
	call move2(0.1, 0.5)
	call draw2(0.9, 0.5)
	call move2(0.5, 0.1)
	call draw2(0.5, 0.9)

	call textang(ang)
	call color(MAGENTA)
	call move2(0.5, 0.5)
	call topjustify
	call drawchar('B')
	call textang(0.0)
	call textjustify(0)
	call boxtext(0.1, 0.1, 0.4, 0.02, 
     +   'The ''B'' should be topjustified')
	call pushattributes

	call getkey

	call color(BLACK)
	call rect(0.1, 0.1, 0.9, 0.9)
	call color(WHITE)
	call move2(0.1, 0.5)
	call draw2(0.9, 0.5)
	call move2(0.5, 0.1)
	call draw2(0.5, 0.9)

	call textang(ang)
	call color(MAGENTA)
	call move2(0.5, 0.5)
	call topjustify
	call rightjustify
	call drawchar('B')
	call textang(0.0)
	call textjustify(0)
	call boxtext(0.1, 0.1, 0.4, 0.02, 
     +   'The ''B'' should be right/topjustified')

c
	call textjustify(0)

	call getkey

	end
