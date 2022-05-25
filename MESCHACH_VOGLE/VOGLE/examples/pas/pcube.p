program cube;

#include <Vogle.h>

var

	c: char;
	device: string_t;
	r, t, dr, dt: real;
	nkey, nplanes: integer;
	fill, back, backdir: boolean;
	carray: array[1..8, 1..3] of real;

	procedure init;
	begin
		dr := 10.0;
		dt := 0.2;
		carray[1, 1] := -1.0;
		carray[1, 2] := -1.0;
		carray[1, 3] :=  1.0;
		carray[2, 1] :=  1.0;
		carray[2, 2] := -1.0;
		carray[2, 3] :=  1.0;
		carray[3, 1] :=  1.0;
		carray[3, 2] :=  1.0;
		carray[3, 3] :=  1.0;
		carray[4, 1] := -1.0;
		carray[4, 2] :=  1.0;
		carray[4, 3] :=  1.0;
		carray[5, 1] := -1.0;
		carray[5, 2] := -1.0;
		carray[5, 3] := -1.0;
		carray[6, 1] :=  1.0;
		carray[6, 2] := -1.0;
		carray[6, 3] := -1.0;
		carray[7, 1] :=  1.0;
		carray[7, 2] :=  1.0;
		carray[7, 3] := -1.0;
		carray[8, 1] := -1.0;
		carray[8, 2] :=  1.0;
		carray[8, 3] := -1.0
	end;


	(*
	 * drawcube
	 *
	 *	draw the cube setting colours if available
	 *)
	procedure drawcube;
	begin
		if nplanes > 1 then
			Color(RED);

		MakePoly;	(* Front *)
			Move(carray[1][1], carray[1][2], carray[1][3]);
			Draw(carray[2][1], carray[2][2], carray[2][3]);
			Draw(carray[3][1], carray[3][2], carray[3][3]);
			Draw(carray[4][1], carray[4][2], carray[4][3]);
			Draw(carray[1][1], carray[1][2], carray[1][3]);
		ClosePoly;
		
		if nplanes > 1 then
			Color(GREEN);

		MakePoly;	(* Back *)
			Move(carray[6][1], carray[6][2], carray[6][3]);
			Draw(carray[5][1], carray[5][2], carray[5][3]);
			Draw(carray[8][1], carray[8][2], carray[8][3]);
			Draw(carray[7][1], carray[7][2], carray[7][3]);
			Draw(carray[6][1], carray[6][2], carray[6][3]);
		ClosePoly;

		if nplanes > 1 then
			Color(YELLOW);

		MakePoly;	(* Right side *)
			Move(carray[2][1], carray[2][2], carray[2][3]);
			Draw(carray[6][1], carray[6][2], carray[6][3]);
			Draw(carray[7][1], carray[7][2], carray[7][3]);
			Draw(carray[3][1], carray[3][2], carray[3][3]);
			Draw(carray[2][1], carray[2][2], carray[2][3]);
		ClosePoly;

		if nplanes > 1 then
			Color(BLUE);

		MakePoly;	(* Left side *)
			Move(carray[1][1], carray[1][2], carray[1][3]);
			Draw(carray[4][1], carray[4][2], carray[4][3]);
			Draw(carray[8][1], carray[8][2], carray[8][3]);
			Draw(carray[5][1], carray[5][2], carray[5][3]);
			Draw(carray[1][1], carray[1][2], carray[1][3]);
		ClosePoly;

		if nplanes > 1 then
			Color(MAGENTA);

		MakePoly;	(* Top *)
			Move(carray[3][1], carray[3][2], carray[3][3]);
			Draw(carray[7][1], carray[7][2], carray[7][3]);
			Draw(carray[8][1], carray[8][2], carray[8][3]);
			Draw(carray[4][1], carray[4][2], carray[4][3]);
			Draw(carray[3][1], carray[3][2], carray[3][3]);
		ClosePoly;
		
		if nplanes > 1 then
			Color(CYAN);

		MakePoly;	(* Bottom *)
			Move(carray[1][1], carray[1][2], carray[1][3]);
			Draw(carray[5][1], carray[5][2], carray[5][3]);
			Draw(carray[6][1], carray[6][2], carray[6][3]);
			Draw(carray[2][1], carray[2][2], carray[2][3]);
			Draw(carray[1][1], carray[1][2], carray[1][3]);
		ClosePoly
	end;


begin
	init;

	write('Enter output device: ');
	readln(device);

	PrefSize(300, 300);

	Vinit(device);

	nplanes := GetDepth;

	fill := true;
	back := true;
	backdir := true;
	PolyFill(fill);
	BackFace(back);
	BackFaceDir(backdir);

	Color(BLACK);
	Clear;

	Window(-1.5, 1.5, -1.5, 1.5, 9.0, -5.0);
	LookAt(0.0, 0.0, 12.0, 0.0, 0.0, 0.0, 0.0);

	BackFace(true);
	
	if BackBuffer < 0 then
	begin
		VgetDev(device);
		Vexit;
		writeln('cube: device ', device, ' doesn''t support double buffering.');
		halt
	end;

	t := 0.0;
	c := chr(0);

	repeat 
		r := 0.0;
		while r < 360.0 do
		begin
			Color(BLACK);
			Clear;
			PushMatrix;
				Translate(0.0, 0.0, t);
				Rotate(r, 'y');
				Rotate(r, 'z');
				Rotate(r, 'x');
				Color(WHITE);
				drawcube;
				if (nplanes = 1) and fill then
				begin
					PolyFill(false);
					Color(BLACK);
					drawcube;
					PolyFill(fill)
				end;

			PopMatrix;

			if c = 'p' then
			begin
				VnewDev(device);
				c := chr(0)
			end;

			t := t + dt;
			if (t > 3.0) or (t < -18.0) then
				dt := -dt;

			SwapBuffers;

			c := chr(CheckKey);
			if c <> chr(0) then
			begin
				if c = 'p' then begin
					Voutput('cube.ps');
					VnewDev('postscript')
				end else if c = 'b' then begin
					back := not back;
					BackFace(back);
				end else if c = 'f' then begin
					fill := not fill;
					PolyFill(fill);
				end else if c = 'd' then begin
					backdir := not backdir;
					BackFaceDir(backdir);
				end else begin
					Vexit;
					halt
				end
			end;

			r := r + dr
		end
	until(false);
end.
