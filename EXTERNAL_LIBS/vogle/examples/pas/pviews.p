program views;
(*
 *	Shows various combinations of viewing and
 *	projection transformations.
 *)

#include "Vogle.h"

var
	device: string_t;
	i: integer;

	procedure DrawTetra;
	(*
	 *	generate a tetraedron as a series of move draws
	 *)
	begin
		Move(-0.5,  0.866, -0.5);
		Draw(-0.5, -0.866, -0.5);
		Draw( 1.0,  0.0,   -0.5);
		Draw(-0.5,  0.866, -0.5);
		Draw( 0.0,  0.0,    1.5);
		Draw(-0.5, -0.866, -0.5);
		Move( 1.0,  0.0,   -0.5);
		Draw( 0.0,  0.0,    1.5);
		
		(* 
		 * Label the vertices.
		 *)
		Color(WHITE);
		TextSize(0.3, 0.5);		(* set the text size *)
		Move(-0.5,  0.866, -0.5);
		DrawChar('a');
		Move(-0.5, -0.866, -0.5);
		DrawChar('b');
		Move( 1.0,  0.0,   -0.5);
		DrawChar('c');
		Move( 0.0,  0.0,    1.5);
		DrawChar('d')
	end;

begin
	write('Enter device name: ');
	readln(device);
	Vinit(device);

	Color(BLACK);
	Clear;

	(*
	 * we want to draw just within the boundaries of the screen
	 *)
	ViewPort(-0.9, 0.9, -0.9, 0.9);


	Ortho2(-5.0, 5.0, -5.0, 5.0);	(* set the world size *)

	Color(RED);
	Rect(-5.0, -5.0, 5.0, 5.0);	(* draw a boundary frame *)

	(*
	 * set up a perspective projection with a field of view of
	 * 40.0 degrees, aspect ratio of 1.0, near clipping plane 0.1,
	 * and the far clipping plane at 1000.0.
	 *)
	Perspective(40.0, 1.0, 0.1, 1000.0);

	(*
	 * we want the drawing to be done with our eye point at (5.0, 8.0, 5.0)
	 * looking towards (0.0, 0.0, 0.0). The last parameter gives a twist
	 * in degrees around the line of sight, in this case zero.
	 *)
	LookAt(5.0, 8.0, 5.0, 0.0, 0.0, 0.0, 0.0);

	DrawTetra;

	Move2(-4.5, -4.5);
	TextSize(0.6, 0.9);		(* set the text size *)
	DrawStr('perspective/lookat');

	i := GetKey;

	(*
	 * window can also be used to give a perspective projection. Its
	 * arguments are 6 clipping planes, left, right, bottom, top, near,
	 * and far.
	 *)
	Window(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
	(*
	 * as window replaces the current transformation matrix we must
	 * specify our viewpoint again.
	 *)
	LookAt(5.0, 8.0, 5.0, 0.0, 0.0, 0.0, 0.0);

	Color(BLACK);
	Clear;

	Color(GREEN);
	Rect(-5.0, -5.0, 5.0, 5.0);

	DrawTetra;

	Move2(-4.5,-4.5);
	TextSize(0.6, 0.9);		(* set the text size *)
	DrawStr('window/lookat');

	i := GetKey;

	(*
	 * set up our original perspective projection again.
	 *)
	Perspective(40.0, 1.0, 0.1, 1000.0);
	(*
	 * polarview also specifies our viewpoint, but, unlike lookat, in polar
	 * coordinates. Its arguments are the distance from the world origin, an
	 * azimuthal angle in the x-y plane measured from the y axis, an 
	 * incidence angle in the y-z plane measured from the z axis, and a
	 * twist around the line of sight.
	 *)
	PolarView(15.0, 30.0, 30.0, 30.0);

	Color(BLACK);
	Clear;

	Color(MAGENTA);
	Rect(-5.0, -5.0, 5.0, 5.0);

	DrawTetra;

	Move2(-4.5,-4.5);
	TextSize(0.6, 0.9);		(* set the text size *)
	DrawStr('perspective/polarview');

	i := GetKey;

	(*
	 * once more with window for comparison
	 *)
	Window(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
	PolarView(6.0, 20.0, -30.0, 70.0);

	Color(BLACK);
	Clear;

	Color(YELLOW);
	Rect(-5.0, -5.0, 5.0, 5.0);

	DrawTetra;

	Move2(-4.5,-4.5);
	TextSize(0.6, 0.9);		(* set the text size *)
	DrawStr('window/polarview');

	i := GetKey;

	Vexit;
end.

