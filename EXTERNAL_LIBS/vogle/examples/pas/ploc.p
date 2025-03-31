program loc;

#include "Vogle.h"

	var dev, name: string_t;
	    i, bt, curpnt, act, nchars: integer;
	    x, y, sx, sy: real;

begin
	write('Enter device name: ');
	readln(dev);

	PrefSize(512, 512);
	Vinit(dev);

	Color(BLACK);
	Clear;

	Color(BLUE);

	(*
	 * draw some axes
	 *)
	Move2(0.0, 1.0);
	Draw2(0.0, -1.0);

	Move2(1.0, 0.0);
	Draw2(-1.0, 0.0);

	Color(GREEN);

	act := 0;
	curpnt := 0;
	(*
	 * locator returns whether a mouse button has been
	 * pressed or not. In a device such as the tektronix
	 * where you have to wait for a keypress to get the
	 * position of the crosshairs locator returns 0
	 * automatically on every second call. A return value
	 * of 2 indicates the second mouse button has been pressed.
	 * A return value of 1 indicates the first mouse button has
	 * been pressed. We wait for the locator to return zero so
	 * that we know the mouse button has been released.
	 *)
	bt := 0;
	while (bt <> 2) do
	    begin
		bt := Locator(x, y);
		if (bt = -1) then
		    begin
			Vexit;
			writeln('No locator device found');
			halt;
		    end
		else if (bt = 0) then
			act := 1
		else if (act <> 0) then
		    begin
			act := 0;
			if (bt = 1) then
			    begin
				if (curpnt <> 0) then
				    begin
					Move2(sx, sy);
					Draw2(x, y);
					curpnt := 0
				    end
				else
					curpnt := 1;
				sx := x;
				sy := y
			    end
		    end
	    end;

	Vexit

end.
