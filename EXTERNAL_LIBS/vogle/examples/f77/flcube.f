
	program flcube
	parameter(TRANS = 20.0, SC = 0.1)

        integer BLACK, FACE, FILLED, OUTLINE
        parameter (BLACK = 0, FACE = 1, FILLED = 2, OUTLINE = 3)

	character *10 device, but*1
	integer getdepth, checkkey, slocator, backbuffer

	logical back, fill, hatch

c	print*, 'Enter output device:'
c	read(*, '(a)') device
	device=' '

	call prefposition(50, 50)
	call prefsize(500, 500)

	call vinit(device)

	call window(-800.0, 800.0, -800.0, 800.0, -800.0, 800.0)
	call lookat(0.0, 0.0, 1500.0, 0.0, 0.0, 0.0, 0.0)

	tdir = TRANS
	scal = SC

c
c Start with a very ordinary filled cube like in the original demo...
c
	call polyhatch(.false.)
	call hatchang(45.0)
	call hatchpitch(40.0)
	call polyfill(.true.)

	fill = .true.
	hatch = .false.
	back = .true.

	call makeobj(FACE)
		call makepoly
			call rect(-200.0, -200.0, 200.0, 200.0)
		call closepoly
	call closeobj

	call makecube(FILLED)

	nplanes = getdepth()
	if (nplanes .eq. 1) call makecube(OUTLINE)

	call backface(back)
c
c Setup drawing into the backbuffer....
c
	if (backbuffer().lt.0) then
		call vexit
		write(*,*)'Device can''t support doublebuffering'
		stop
	endif

1	continue
		idum = slocator(x, y)
		call pushmatrix
			call rotate(100.0 * x, 'y')
			call rotate(100.0 * y, 'x')
			call color(BLACK)
			call clear
			call callobj(FILLED)
			if (nplanes .eq. 1 .and. (fill .or. hatch))
     +				call callobj(OUTLINE)
		call popmatrix
		call swapbuffers

		
		but = char(checkkey())
		if (but .eq. 'x') then
			call translate(tdir, 0.0, 0.0)
		else if (but .eq. 'y') then
			call translate(0.0, tdir, 0.0)
		else if (but .eq. 'z') then
			call translate(0.0, 0.0, tdir)
		else if (but .eq. 's') then
			call scale(scal, scal, scal)
		else if (but .eq. 'f') then
			fill = .not. fill
			hatch = .false.
			call polyfill(fill)
		else if (but .eq. 'h') then
			hatch = .not. hatch
			fill = .false.
			call polyhatch(hatch)
		else if (but .eq. 'b') then
			back = .not. back
			call backface(back)
		else if (but .eq. '-') then
			tdir = -tdir
			
			if (scal .lt. 1.0) then
				scal = 1.0 + SC
			else
				scal = 1.0 - SC
			end if

		else if (but .eq. '+') then
			tdir = TRANS
		else if (but .eq. 'q' .or. ICHAR(but) .eq. 27) then
			call vexit
			stop
		end if

	goto 1
	end

	subroutine makecube(obj)
	integer	obj
        integer BLACK, RED, GREEN, YELLOW, BLUE, CYAN,
     +          MAGENTA, WHITE, OUTLINE, FILLED, FACE
        parameter (BLACK = 0, RED = 1, GREEN = 2, YELLOW = 3,
     +		 BLUE = 4, CYAN = 5, MAGENTA = 6, WHITE = 7)
	parameter(FACE = 1, FILLED = 2, OUTLINE = 3)

	call makeobj(obj)
		if (obj .eq. OUTLINE) then
			call pushattributes
			call color(BLACK)
			call polyfill(.false.)
			call polyhatch(.false.)
		end if

		call pushmatrix
			call translate(0.0, 0.0, 200.0)
			if (obj .eq. FILLED) call color(RED)
			call callobj(FACE)
		call popmatrix

		call pushmatrix
			call translate(200.0, 0.0, 0.0)
			call rotate(90.0, 'y')
			if (obj .eq. FILLED) call color(GREEN)
			call callobj(FACE)
		call popmatrix

		call pushmatrix
			call translate(0.0, 0.0, -200.0)
			call rotate(180.0, 'y')
			if (obj .eq. FILLED) call color(BLUE)
			call callobj(FACE)
		call popmatrix

		call pushmatrix
			call translate(-200.0, 0.0, 0.0)
			call rotate(-90.0, 'y')
			if (obj .eq. FILLED) call color(CYAN)
			call callobj(FACE)
		call popmatrix

		call pushmatrix
			call translate(0.0, 200.0, 0.0)
			call rotate(-90.0, 'x')
			if (obj .eq. FILLED) call color(MAGENTA)
			call callobj(FACE)
		call popmatrix

		call pushmatrix
			call translate(0.0, -200.0, 0.0)
			call rotate(90.0, 'x')
			if (obj .eq. FILLED) call color(YELLOW)
			call callobj(FACE)
		call popmatrix

		if (obj .eq. OUTLINE) call popattributes

	call closeobj

	return
	end
