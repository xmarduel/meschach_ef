program tetra;
(*
 * Demonstrate a rotating translating tetrahedron.
 *)

#include 'Vogle.h'

const
	TETRAHEDRON =	1;
	NSIDES =	3;
	NFACES =	4;
	NPNTS =		4;

var
	
	pnts: array[1..NPNTS, 1..3] of real;
	faces: array[1..NFACES, 1..NSIDES] of integer;
	colface: array[1..4] of integer;

	device, dev: string_t;
	i: integer;
	rotval, drotval, zeye, R, tx, tz: real;
	do_backface, do_backface_dir, do_fill: boolean;
	c: integer;

	procedure init;
	begin
		rotval := 0.0;
		drotval := 5.0;
		zeye := 5.0;
		R := 1.6;
		tx := 0.0;
		tz := R;
		do_backface := false;
		do_backface_dir := false;
		do_fill := false;
		pnts[1, 1] := -0.5;
		pnts[1, 2] :=  0.866;
		pnts[1, 3] := -0.667;
		pnts[2, 1] := -0.5;
		pnts[2, 2] := -0.866;
		pnts[2, 3] := -0.667;
		pnts[3, 1] :=  1.0;
		pnts[3, 2] :=  0.0;
		pnts[3, 3] := -0.667;
		pnts[4, 1] :=  0.0;
		pnts[4, 2] :=  0.0;
		pnts[4, 3] :=  1.334;

		faces[1, 1] := 3;
		faces[1, 2] := 2;
		faces[1, 3] := 1;
		faces[2, 1] := 1;
		faces[2, 2] := 2;
		faces[2, 3] := 4;
		faces[3, 1] := 2;
		faces[3, 2] := 3;
		faces[3, 3] := 4;
		faces[4, 1] := 3;
		faces[4, 2] := 1;
		faces[4, 3] := 4;

		colface[1] := GREEN;
		colface[2] := YELLOW;
		colface[3] := CYAN;
		colface[4] := MAGENTA;
	end;

	(*
	 * maketheobject
	 *
	 *	generate a tetraedron as a series of move draws
	 *)
	procedure maketheobject;
	var
		i, j: integer;
		x, y, z: real;
	begin

		MakeObj(TETRAHEDRON);

			for i := 1 to NFACES do
			begin
				Color(colface[i]);
				MakePoly;
					x := pnts[faces[i, 1], 1];
					y := pnts[faces[i, 1], 2];
					z := pnts[faces[i, 1], 3];
					Move(x, y, z);
					for j := 2 to NSIDES do
					begin
						x := pnts[faces[i, j], 1];
						y := pnts[faces[i, j], 2];
						z := pnts[faces[i, j], 3];
						Draw(x, y, z);
					end;
				ClosePoly;
			end;

		CloseObj;
	end;

begin

	init;
	PrefSize(400, 400);

	Vinit(device);          (* set up device *)

	do_fill := true;
	do_backface := true;
	do_backface_dir := true;

	Font('small');

	(*
	 * Make the tetrahedral object
	 *)
	maketheobject;

	(*
	 * See what to do with the thingo...
	 *)
	PolyFill(do_fill);
	BackFace(do_backface);
	BackFaceDir(do_backface_dir);

	(*
	 * set up a perspective projection with a field of view of
	 * 40.0 degrees, aspect ratio of 1.0, near clipping plane 0.1,
	 * and the far clipping plane at 1000.0.
	 *)
	Perspective(40.0, 1.0, 0.001, 15.0);
	LookAt(0.0, 0.0, zeye, 0.0, 0.0, 0.0, 0.0);

	(*
	 * Setup drawing into the backbuffer....
	 *)

	if (BackBuffer < 0) then
	begin
		VgetDev(dev);
		Vexit;
		writeln(, 'The device ', dev, ' can''t support doublebuffering');
		halt;
	end;

	repeat
		for i:= 0 to 72 do
		begin
			rotval := i * drotval;
			Color(BLACK);
			Clear;

			(*
			 * Rotate the whole scene...(this acumulates - hence
			 * drotval)
			 *)
			Rotate(drotval * 0.1, 'x');
			Rotate(drotval * 0.1, 'z');

			Color(RED);
			PushMatrix;
				PolyFill(false);
				Rotate(90.0, 'x');
				Circle(0.0, 0.0, R);
				PolyFill(do_fill);
			PopMatrix;

			Color(BLUE);
			Move(0.0, 0.0, 0.0);
			Draw(tx, 0.0, tz);
			
			(*
			 * Remember! The order of the transformations is
			 * the reverse of what is specified here in between
			 * the pushmatrix and the popmatrix. These ones don't
			 * accumulate because of the push and pop.
			 *)
			PushMatrix;
				Translate(tx, 0.0, tz);
				Rotate(rotval, 'x');
				Rotate(rotval, 'y');
				Rotate(rotval, 'z');
				Scale(0.4, 0.4, 0.4);
				CallObj(TETRAHEDRON);
			PopMatrix;

			tz := R * cos((rotval * 3.1415926535 / 180));
			tx := R * sin((rotval * 3.1415926535 / 180));

			SwapBuffers;

			c := CheckKey;
			if c = ord('b') then begin
				do_backface := not do_backface;
				BackFace(do_backface)
			end else if c = ord('d') then begin
				do_backface_dir := not do_backface_dir;
				BackFaceDir(do_backface_dir)
			end else if c = ord('f') then begin
				do_fill := not do_fill;
				PolyFill(do_fill)
			end else if c <> 0 then begin
				Vexit;
				halt;
			end
		end;

	until false;

end.
