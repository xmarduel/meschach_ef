program getstr;
(*
 * Test the getting of strings in graphics mode.
 *)
#include 'Vogle.h'


var
	device: string_t;
	cw, ch: real;
	i, n: integer;
	buf: array [1..10] of string_t;
	line: string_t;

begin

	write('Enter output device: ');
	readln(device);

	Vinit(device);

	VgetDev(line);
	writeln('Vgetdev returns: ', line);

	Clipping(false);

	Window(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0);
	LookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0);

	TextSize(0.1, 0.25);


	Rotate(30.0, 'x');
	Rotate(30.0, 'z');
	Rotate(60.0, 'y');

	Color(BLACK);
	Clear;
	Color(YELLOW);

	Rect(-0.5, -0.5, 0.5, 0.5);
	Move2(-0.5, 0.0);

	Color(GREEN);

	n := 1;

	repeat
		i := GetString(BLACK, buf[n]);
		if (i > 0) and (n < 10) then 
			n := n + 1

	until (i <= 0) or (n > 10);
			
	Vexit;

	for i := 1 to n - 1 do
		writeln('Line ', i, ' was: ', buf[i])

end.
