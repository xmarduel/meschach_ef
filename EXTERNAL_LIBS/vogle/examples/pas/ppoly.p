program poly;

#include "Vogle.h"

var	
	device: string_t;
	parray: Poly3_array_t;
	i: integer;

	procedure init;
	(*
	 * Don't ya just love pascal....
	 *)
	begin
		parray[1, 1] := -8.0;
		parray[1, 2] := -8.0;
		parray[1, 3] :=  0.0;
		parray[2, 1] := -5.0;
		parray[2, 2] := -8.0;
		parray[2, 3] :=  0.0;
		parray[3, 1] := -5.0;
		parray[3, 2] := -5.0;
		parray[3, 3] :=  0.0;
		parray[4, 1] := -8.0;
		parray[4, 2] := -5.0;
		parray[4, 3] :=  0.0
	end;

	procedure drawpoly;
	begin
		Color(YELLOW);

		(*
		 * Draw a polygon using poly, parray is our array of
		 * points and 4 is the number of points in it.
		 *)
		Poly(4, parray);

		Color(GREEN);

		(*
		 * Draw a 5 sided figure by using move, draw and closepoly.
		 *)
		MakePoly;
			Move(0.0, 0.0, 0.0);
			Draw(3.0, 0.0, 0.0);
			Draw(3.0, 4.0, 0.0);
			Draw(-1.0, 5.0, 0.0);
			Draw(-2.0, 2.0, 0.0);
		ClosePoly;

		Color(MAGENTA);

		(*
		 * draw a sector representing a 1/4 circle
		 *)
		Sector(1.5, -7.0, 3.0, 0.0, 90.0);

		i := GetKey
	end;

begin
(*
 * Using polygons, hatching, and filling.
 *)
	init;
	write('Enter output device: ');
	readln(device);

	Voutput('ppoly.ps');
	Vinit(device);

	Color(BLACK);		/* clear to bleck */
	Clear;

	(*
	 * world coordinates are now in the range -10 to 10
	 * in x, y, and z. Note that positive z is towards us.
	 *)
	Ortho(-10.0, 10.0, -10.0, 10.0, 10.0, -10.0);

	Color(YELLOW);

	(*
	 * write out the string "Polygon from poly()" in the
	 * starting at (-8.0, -4.0) and scaled to be 4.0 units long,
	 * 0.5 units high.
	 *)
	BoxText(-8.0, -4.0, 4.0, 0.5, 'Polygon from poly()');

	Color(GREEN);

	(*
	 * write out a scaled string starting at (0.0, 6.0)
	 *)
	BoxText(0.0, 6.0, 4.0, 0.5, 'Polygon from move()/ draw()');

	Color(MAGENTA);

	(*
	 * write out a scaled string starting at (0.0, 6.0)
	 *)
	BoxText(3.5, -3.5, 1.9, 0.5, 'Sector');

	drawpoly;		(* draw some polygons *)

	(*
	 * turn on polygon hatching
	 *)
	PolyHatch(true);
	HatchAng(45.0);
	HatchPitch(0.3);

	(*
	 *  Rotate 20 degrees around x and 30 around y
	 *)
	Rotate(20.0, 'x');
	Rotate(30.0, 'y');

	drawpoly;		(* draw some polygons with hatching *)

	(*
	 * turn on polygon filling - this automatically turns off hatching
	 *)
	PolyFill(true);

	(*
	 *  Do another set of rotations.
	 *)
	Rotate(20.0, 'x');
	Rotate(30.0, 'y');

	drawpoly;		(* draw some polygons with filling *)

	Vexit
end.
