      interface to subroutine C_setdash[c](x)
      real *8 x
      end

      interface to subroutine C_linestyle[c](s[reference])
      character *(*) s
      end

      interface to subroutine C_draw[c](x, y, z)
      real *8 x, y, z
      end

      interface to subroutine C_draw2[c](x, y)
      real *8 x, y
      end

      interface to subroutine C_rdraw[c](x, y, z)
      real *8 x, y, z
      end

      interface to subroutine C_rdraw2[c](x, y)
      real *8 x, y
      end

      interface to subroutine C_sdraw2[c](x, y)
      real *8 x, y
      end

      interface to subroutine C_rsdraw2[c](x, y)
      real *8 x, y
      end

      subroutine setdash(x)
      call C_setdash(x)
      end

      subroutine linestyle(s)
      character *(*) s
      character *128 t_string

      call C_linestyle(t_string(s))
      end

      subroutine draw(x, y, z)
      call C_draw(x, y, z)
      end

      subroutine draw2(x, y)
      call C_draw2(x, y)
      end

      subroutine rdraw(x, y, z)
      call C_rdraw(x, y, z)
      end

      subroutine rdraw2(x, y)
      call C_rdraw2(x, y)
      end

      subroutine sdraw2(x, y)
      call C_sdraw2(x, y)
      end

      subroutine rsdraw2(x, y)
      call C_rsdraw2(x, y)
      end
