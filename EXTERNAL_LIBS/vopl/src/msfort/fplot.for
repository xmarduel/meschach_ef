	interface to subroutine C_plot2[c](x[reference], 
     +	y[reference], n)
	real	x(1), y(1)
	integer *2 n
	end

	subroutine plot2(x, y, n)
	real x(n), y(n)
	
	call C_plot2(x, y, n)
	end
