c
c	Another simple test program for vopl.
c
c 	This one tries to show the various "fit" options
c
	program test3
	parameter (n = 10)

	real x(n), y(n)

	character device*30
	
	data x/1.0, 2.0, 3.0, 6.0, 17.0, 19.0, 23.0, 45.0, 50.0, 56.0/
	data y/1.0, 3.0, 5.0, 9.0, 17.0, 45.0, 23.0, 99.0, 50.0, 20.0/
c
c	Get VOGLE device
c
	write(*,'(a,$)') 'Enter VOGLE device: '
	read(*,'(a)') device

c
c	First we'll do a linear least square fit.
c
	call fit(2)
	call degree(1)
c
c	Adjust the scaling according to x and y arrays
c
	call adjustscale(x, n, 'x')
	call adjustscale(y, n, 'y')
c
c	Give it a title
c
	call graphtitle('Linear Least square fit')
c
c	As we are now about to do some graphics we initialise VOGLE
c	and clear to BLACK
c
	call vinit(device)
	call color(0)
	call clear
c
c	Draw the title in CYAN
c
	call color(6)
	call drawtitle
c
c	Now set the color to GREEN
c
	call color(2)

c
c	Draw the default set of axes (in GREEN)
c
	call drawaxes2
c
c	Set color to RED
c
	call color(1)
c
c	Change to the "markers" font and set the current marker string
c
	call font('markers')
	call marker('a')
c
c	Draw the Graph
c
	call plot2(x, y, n)
c
c	Wait around a bit
c
	call getkey
c
c	Now we'll do a second order fit.
c
	call degree(2)
	call graphtitle('Second order least square fit')

	call color(0)
	call clear

	call color(7)
	call plot2(x, y, n)
c
c	Change back to the "text" type font to draw the title and axes
c
	call font('futura.m')

	call color(3)
	call drawaxes2

	call color(6)
	call drawtitle
c
c 	Wait a bit
c
	call getkey
c
c	Now do a Cubic spline fit (cardinal spline for this one)
c
	call fit(3)
	
	call color(0)
	call clear

	call color(5)
	call drawaxes2

	call graphtitle('Cardinal Cubic Spline Fit')
	call color(6)
	call drawtitle

c
c	Note, we haven't changed to the Marker font here
c
	call plot2(x, y, n)
c
	call getkey

	call vexit
	end
