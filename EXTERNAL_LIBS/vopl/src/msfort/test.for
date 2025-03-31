
	program test
c
c Test vopl Fortran interfaces
c
	character *40 title
	character *1 ax

	real x(5), y(5)
	data x/0.0, 1.0, 2.0, 3.0, 4.0/
	data y/10.0, 11.0, 12.0, 13.0, 14.0/

	title = 'A Simple Test Title'
	ax = 'x'
	n = 5

	print*,'Calling axistitle(title, ax)'
	call axistitle(title, ax)

	print*,'Calling drawaxis(ax)'
	call drawaxis(ax)

	print*,'Calling axistitle(''Bonky'', ax)'
	call axistitle('Bonky', ax)

	print*,'Calling drawaxis(''x'')'
	call drawaxis('x')
	
	print*,'Calling drawaxes2'
	call drawaxes2

	print*,'Calling drawaxes'
	call drawaxes

	print*,'Calling plot2(x, y, n)'
	call plot2(x, y, n)

	print*,'Calling adjustscale(x, n, ax)'
	call adjustscale(x, n, ax)

	print*,'Calling adjustscale(x, 5, ''y'')'
	call adjustscale(y, 5, 'y')

	print*,'Calling fit(2)'
	call fit(2)

	print*,'Calling fit(n)'
	call fit(n)

	print*,'Calling skip(2)'
	call skip(2)

	print*,'Calling skip(n)'
	call skip(n)

	print*,'Calling endslopes(1.0, 11.0)'
	call endslopes(1.0, 11.0)

	a = 2.0
	b = 33.3
	print*,'Calling endslopes(a = 2.0, b = 33.3)'
	call endslopes(a, b)

	print*,'Calling gridspacing(2)'
	call gridspacing(2)

	print*,'Calling gridspacing(n)'
	call gridspacing(n)

	print*,'Calling scaling(2, ax)'
	call scaling(2, ax)

	print*,'Calling scaling(n, ax)'
	call scaling(n, ax)

	print*,'Calling scaling(n, ''x'')'
	call scaling(n, 'x')

	print*,'Calling scaling(3, ''x'')'
	call scaling(3, 'x')

	print*,'Calling tickmarks(2, ax)'
	call tickmarks(2, ax)

	print*,'Calling tickmarks(n, ax)'
	call tickmarks(n, ax)

	print*,'Calling stickmarks(n, ''x'')'
	call tickmarks(n, 'x')

	print*,'Calling tickmarks(3, ''x'')'
	call tickmarks(3, 'x')

	print*,'Calling tickspacing(2, ax)'
	call tickspacing(2, ax)

	print*,'Calling tickspacing(n, ax)'
	call tickspacing(n, ax)

	print*,'Calling stickspacing(n, ''x'')'
	call tickspacing(n, 'x')

	print*,'Calling tickspacing(3, ''x'')'
	call tickspacing(3, 'x')

	print*,'Calling annotate(''%f.3'', ax)'
	call annotate('%f.3', ax)

	print*,'Calling annotate(title, ax)'
	call annotate(title, ax)

	print*,'Calling annotate(title, ''x'')'
	call annotate(title, 'x')

	print*,'Calling arrayindex(10)'
	call arrayindex(10)

	print*,'Calling arrayindex(n)'
	call arrayindex(n)

	print*,'Calling axisintersection(1.0, 11.0)'
	call axisintersection(1.0, 11.0)

	a = 2.0
	b = 33.3
	print*,'Calling axisintersection(a = 2.0, b = 33.3)'
	call axisintersection(a, b)

	print*,'Calling marker(''Hello there'')'
	call marker('Hello there')

	print*,'Calling marker(title)'
	call marker(title)

	print*,'Calling markerspacing(10)'
	call markerspacing(10)

	print*,'Calling markerspacing(n)'
	call markerspacing(n)

	print*,'Calling markerscale(11.5)'
	call markerscale(11.5)

	print*,'Calling markerscale(a)'
	call markerscale(a)

	print*,'Calling graphtitle(''Hello there'')'
	call graphtitle('Hello there')

	print*,'Calling graphtitle(title)'
	call graphtitle(title)

	print*,'Calling drawtitle'
	call drawtitle

	print*,'Calling range(a, b, ax)'
	call range(a, b, ax)

	print*,'Calling range(22.3, 44.2, ax)'
	call range(22.3, 44.2, ax)

	print*,'Calling range(22.3, 44.2, ''y'')'
	call range(22.3, 44.2, 'y')


	print*,'Calling degree(10)'
	call degree(10)

	print*,'Calling degree(n)'
	call degree(n)

	stop
	end
