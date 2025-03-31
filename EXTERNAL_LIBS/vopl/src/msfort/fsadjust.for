	interface to subroutine C_adjustscale[c](x[reference], n, a)
	real x(1)
	integer *2 n
	character *1 a
	end

	subroutine adjustscale(x, n, a)
	real x(n)
	character *1 a
	call C_adjustscale(x, n, a)
	end
