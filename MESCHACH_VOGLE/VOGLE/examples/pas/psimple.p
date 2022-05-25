
(*
 * A program showing basic line drawing, text and (if applicable)
 * colour. As none of the projection routines have been called we
 * move and draw in the initial coordinate system -1.0 to 1.0.
 *)
program simple(input, output);

#include "Vogle.h"

var
	device: string_t;
	p: string_t;
	i: integer;

begin
	p := 'Hello, world';

	Voutput('psimple.ps');
	write('Enter output device: ');
	readln(device);

	PrefSize(300, 300);
	PrefPosition(100, 100);
	Vinit(device);		(* set up device *)

	Color(BLACK);		(* set current color *)
	Clear;		(* clear screen to current color *)

	Color(GREEN);
			(* 2 d move to start where we want drawstr to start *)
	Move2(-0.9, 0.9);

	DrawStr('A Simple Example');	(* draw string in current color *)

	(*
	 * the next four lines draw the x 
	 *)
	Move2(0.0, 0.0);
	Draw2(0.76, 0.76);
	Move2(0.0, 0.76);
	Draw2(0.76, 0.0);

	Move2(0.0, 0.5);
	DrawStr('x done');
	DrawStr('next sentence');

	Move2(0.0, 0.1);
	for i := 1 to 11 do
		DrawChar(p[i]);		(* draw the string one char at a time *)

	(*
	 * the next five lines draw the square
	 *)
	Move2(0.0, 0.0);
	Draw2(0.76, 0.0);
	Draw2(0.76, 0.76);
	Draw2(0.0, 0.76);
	Draw2(0.0, 0.0);

	i := GetKey;		(* wait for some input *)

	Vexit		(* set the screen back to its original state *)
end.
