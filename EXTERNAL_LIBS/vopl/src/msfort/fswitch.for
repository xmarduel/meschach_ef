	interface to subroutine C_fit[c](type)
	integer *2 type
	end

	interface to subroutine C_skip[c](i)
	integer *2 i
	end

	interface to subroutine C_endslopes[c](a, b)
	real *8 a, b
	end

	interface to subroutine C_gridspacing[c](i)
	integer *2 i
	end

	interface to subroutine C_scaling[c](i, a)
	integer *2 i
	character *1 a
	end

	interface to subroutine C_tickmarks[c](i, a)
	integer *2 i
	character *1 a
	end

	interface to subroutine C_tickspacing[c](i, a)
	integer *2 i
	character *1 a
	end

	interface to subroutine C_minorticks[c](i, a)
	integer *2 i
	character *1 a
	end

	interface to subroutine C_annotate[c](f[reference], a)
	character *(*) f
	character *1 a
	end

	interface to subroutine C_arrayindex[c](i)
	integer *2 i
	end

	interface to subroutine C_marker[c](s[reference])
	character *(*) s
	end

	interface to subroutine C_markerspacing[c](i)
	integer *2 i
	end

	interface to subroutine C_markerscale[c](i)
	real *8 i
	end

	interface to subroutine C_graphtitle[c](s[reference])
	character *(*) s
	end

	interface to subroutine C_drawtitle[c]
	end

	interface to subroutine C_range[c](a, b, ax)
	real *8 a, b
	character *1 ax
	end

	subroutine fit(i)
	call C_fit(i)
	end

	subroutine skip(i)
	call C_fit(i)
	end

	subroutine endslopes(a, b)
	call C_endslopes(a, b)
	end

	subroutine gridspacing(i)
	call C_gridspacing(i)
	end

	subroutine scaling(i, ax)
	character *1 ax
	call C_scaling(i, ax)
	end

	subroutine tickmarks(i, ax)
	character *1 ax
	call C_tickmarks(i, ax)
	end

	subroutine tickspacing(i, ax)
	character *1 ax
	call C_tickspacing(i, ax)
	end

	subroutine minorticks(i, ax)
	character *1 ax
	call C_minorticks(i, ax)
	end

	subroutine annotate(f, ax)
	character *(*) f
	character *1 ax
	character *128 t_string
	call C_annotate(t_string(f), ax)
	end

	subroutine arrayindex(i)
	call C_arrayindex(i)
	end

	subroutine axisintersection(a, b)
	call C_axisintersection(a, b)
	end

	subroutine marker(s)
	character *(*) s
	character *128 t_string
	call C_marker(t_string(s))
	end

	subroutine markerspacing(i)
	call C_markerspacing(i)
	end

	subroutine markerscale(a)
	call C_markerscale(a)
	end

	subroutine graphtitle(s)
	character *(*) s
	character *128 t_string
	call C_graphtitle(t_string(s))
	end

	subroutine drawtitle
	call C_drawtitle
	end

	subroutine range(a, b, ax)
	character *1 ax
	call C_range(a, b, ax)
	end
