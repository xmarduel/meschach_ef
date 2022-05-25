
program trivial;

#include "Vogle.h"
(*
 * the basic test program for a driver if we can draw a line and do
 * hardware text we are almost there!
 *)
var
	device: string_t;
	i: integer;

begin
	write('Enter output device: ');   (* read in device name *)
	readln(device);

	Vinit(device);

	Font('large');

	Color(BLACK);			(* we want to clear in black *)
	Clear;				(* clear to current color *)

	Color(GREEN);			(* set current color to green *)

	Move2(-1.0, 0.0);		(* draw a horizontal line at y = 0 *)
	Draw2(1.0, 0.0);

	i := GetKey;			(* pause for some input *)

	Move2(0.0, 0.0);		(* draw a line along x = 0 *)
	Draw2(0.0, 1.0);

	Move2(0.0, 0.0);		(* move to the middle of the screen *)
	DrawStr('Hello');		(* draw "Hello" starting at the origin *)

	i := GetKey;			(* pause again *)

	Vexit			(* set screen back to original state *)
end.
