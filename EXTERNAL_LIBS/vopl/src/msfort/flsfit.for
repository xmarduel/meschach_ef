	interface to subroutine C_degree[c](i)
	integer *2 i
	end

	subroutine degree(i)
	call C_degree(i)
	end
