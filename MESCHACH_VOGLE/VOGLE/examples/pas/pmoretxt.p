program moretxt;

#include 'Vogle.h'

var
	dev: string_t;
	i: integer;
	x: real;

	(*
	 * drawgrid
	 *
	 *	draw a grid in the middle of the screen
	 *)
	procedure drawgrid;

	var
		x: real;
		i: integer;
	begin

		Color(GREEN);

		Rect(0.1, 0.4, 0.9, 0.6);

		x := 0.2;
	
		for i := 0 to 7 do begin
			Move2(x, 0.4);
			Draw2(x, 0.6);
			x := x + 0.1
		end;

		Move2(0.1, 0.5);
		Draw2(0.9, 0.5);

		Color(YELLOW)
	end;

(*
 * demonstrate some more features of text
 *)

begin
	write('Enter device: ');
	readln(dev);

	Vinit(dev);
	Color(BLACK);
	Clear;

	Ortho2(0.0, 1.0, 0.0, 1.0);

	drawgrid;

	(*
	 * show some scaled text on the grid (In the bottom part)
	 *)
	BoxText(0.1, 0.4, 0.8, 0.1, '{This is Some text] | $');

	i := GetKey;

	Color(BLACK);
	Clear;

	drawgrid;

	(*
	 * centertext causes text to be centered around the current graphics
	 * position this is especially usefull if you want your text to come
	 * out centered on a line, or a character to be centered on a point
	 * in a graph. A non-zero argument turns centertext on.
	 *
	 * show a string centered on the center line
	 *)
	CenterText(true);

	BoxText(0.5, 0.5, 0.8, 0.1, '{This is Some Centered text] | $');

	(*
	 * turn centertext off. We use an argument with the value zero.
	 *)
	CenterText(false);

	i := GetKey;

	Color(BLACK);
	Clear;

	(*
	 * rotate the grid so that it is the same angle as the text after
	 * textang for text ang.
	 *)
	PushMatrix;
		Translate(0.5, 0.5, 0.0);
		Rotate(90.0, 'z');
		Translate(-0.5, -0.5, 0.0);

		drawgrid;
	PopMatrix;

	(*
	 * turn on centered text again
	 *)
	CenterText(true);

	(*
	 * set the angle to 90.
	 *)
	TextAng(90.0);

	(*
	 * draw the string
	 *)
	BoxText(0.5, 0.5, 0.8, 0.1, '{This is Some Rotated Centered text] | $');

	(*
	 * turn off center text
	 *)
	CenterText(false);

	(*
	 * set text angle back to 90
	 *)
	TextAng(0.0);

	i := GetKey;

	Color(BLACK);
	Clear;

	drawgrid;

	(*
	 * as all the software fonts are proportionally spaced we use
	 * the fixedwidth call to make each character take the same amount
	 * of horizontal space. As with centertext this is done by passing
	 * fixedwidth a non-zero argument.
	 *)
	FixedWidth(true);

	BoxText(0.1, 0.5, 0.8, 0.1, '{This is Some Fixedwidth text] | $');

	i := GetKey;

	Color(BLACK);
	Clear;

	drawgrid;

	(*
	 * now try centered and fixewidth at the same time
	 *)
	CenterText(true);

	Move2(0.5, 0.5);
	DrawStr('{This is Some Cent.Fixedwidth text] | $');

	CenterText(false);
	
	i := GetKey;
	Color(BLACK);
	Clear;

	drawgrid;

	(*
	 * scale the text so tha a character is the size of a box in
	 * the grid.
	 *)
	BoxFit(0.8, 0.1, 8);

	(*
	 * draw the two strings fixedwidth (it is still turned on)
	 *)
	Move2(0.1, 0.4);
	DrawStr('ABCDefgh');

	Move2(0.1, 0.5);
	DrawStr('IJKLmnop');

	i := GetKey;

	Vexit
end.
