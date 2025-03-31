	interface to subroutine C_axistitle[c]
     + (title[reference], ax)
	character *(*) title
	character *1 ax
	end

	interface to subroutine C_drawaxis[c](ax)
	character *1 ax
	end

	interface to subroutine C_drawaxes2[c]
	end

	interface to subroutine C_drawaxes[c]
	end

	subroutine axistitle(t, ax)
	character *(*) t
	character *128 t_string
	character *1 ax
	
	call C_axistitle(t_string(t), ax)
	end

	subroutine drawaxis(ax)
	character *1 ax
	call C_drawaxis(ax)
	end

	subroutine drawaxes2
	call C_drawaxes2
	end

	subroutine drawaxes
	call C_drawaxes
	end


