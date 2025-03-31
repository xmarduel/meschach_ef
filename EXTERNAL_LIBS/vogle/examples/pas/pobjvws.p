program objvws;

#include "Vogle.h"


const
	CUBE = 1;
	TOPLEFT = 2;
	TOPRIGHT = 3;
	BOTTOMLEFT = 4;
	BOTTOMRIGHT = 5;

var
	device: string_t;
	i:	integer;

	(*
	 * side
	 *
	 *	define a face for the cube
	 *)
	procedure side;
	begin
		PushMatrix;
			Translate(0.0, 0.0, 1.0);
			Rect(-1.0, -1.0, 1.0, 1.0);
		PopMatrix;
	end;

	(*
	 * makecube
	 *
	 *	set up a cube
	 *)
	procedure makecube;
	begin
		MakeObj(CUBE);

			(*
			 * The border around the cube
			 *)
			Rect(-5.0, -5.0, 10.0, 10.0);

			(*
			 * Make the cube from 4 squares
			 *)
			PushMatrix;
				side;
				Rotate(90.0, 'x');
				side;
				Rotate(90.0, 'x');
				side;
				Rotate(90.0, 'x');
				side;
			PopMatrix;

		CloseObj;
	end;

begin

	write('Enter device name: ');
	readln(device);
	Vinit(device);

	PushViewPort;

	TextSize(0.5, 0.9);
	Font('futura.m');

	Color(BLACK);
	Clear;

	makecube;

	(*
	 * set up an object which draws in the top left of the screen.
	 *)
	MakeObj(TOPLEFT);
		ViewPort(-1.0, 0.0, 0.0, 1.0);
		Ortho2(-5.0, 5.0, -5.0, 5.0);

		Color(RED);

		Rect(-5.0, -5.0, 5.0, 5.0);

		Perspective(40.0, 1.0, 0.1, 1000.0);
		LookAt(5.0, 8.0, 5.0, 0.0, 0.0, 0.0, 0.0);

		CallObj(CUBE);

		Color(GREEN);

		Move2(-4.5, -4.5);
		DrawStr('perspective/lookat');
	CloseObj;

	(*
	 * now set up one which draws in the top right of the screen
	 *)
	MakeObj(TOPRIGHT);
		ViewPort(0.0, 1.0, 0.0, 1.0);
		Ortho2(-5.0, 5.0, -5.0, 5.0);

		Color(GREEN);

		Rect(-5.0, -5.0, 5.0, 5.0);

		Window(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
		LookAt(5.0, 8.0, 5.0, 0.0, 0.0, 0.0, 0.0);

		CallObj(CUBE);

		Color(RED);

		Move2(-4.5, -4.5);
		DrawStr('window/lookat');
	CloseObj;

	(*
	 * try the bottom left
	 *)
	MakeObj(BOTTOMLEFT);
		ViewPort(-1.0, 0.0, -1.0, 0.0);
		Ortho2(-5.0, 5.0, -5.0, 5.0);

		Color(MAGENTA);

		Rect(-5.0, -5.0, 5.0, 5.0);

		Perspective(40.0, 1.0, 0.1, 1000.0);
		PolarView(15.0, 30.0, 30.0, 30.0);

		CallObj(CUBE);

		Color(YELLOW);

		Move2(-4.5, -4.5);
		DrawStr('perspective/polarview');
	CloseObj;

	(*
	 * and the bottom right
	 *)
	MakeObj(BOTTOMRIGHT);
		ViewPort(0.0, 1.0, -1.0, 0.0);
		Ortho2(-5.0, 5.0, -5.0, 5.0);

		Color(CYAN);

		Rect(-5.0, -5.0, 5.0, 5.0);

		Window(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
		PolarView(8.0, -18.0, -3.0, 18.0);

		CallObj(CUBE);

		Color(BLUE);

		Move2(-4.5, -4.5);
		DrawStr('window/polarview');
	CloseObj;

	(*
	 * now draw them
	 *)
	CallObj(TOPLEFT);
	CallObj(TOPRIGHT);
	CallObj(BOTTOMLEFT);
	CallObj(BOTTOMRIGHT);

	i := GetKey;

	Vexit
end.

