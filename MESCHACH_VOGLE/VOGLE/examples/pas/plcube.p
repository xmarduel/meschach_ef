program lcube;

#include <Vogle.h>

const
	CUBE_SIZE =	200.0;
	TRANS =		25.0;
	SCAL =		0.1;
	FACE =		1;
	FILLED =	2;
	OUTLINE =	3;

var

	device: string_t;
	x, y, tdir, scal: real;
	but, nplanes, i, n: integer;
	back, fill, hatch: boolean;

	procedure makecube(obj: integer);
	begin
		MakeObj(obj);
		if obj = OUTLINE then begin
			PushAttributes;
			PolyFill(false);
			PolyHatch(false);
			Color(BLACK)
		end;

		PushMatrix;
			Translate(0.0, 0.0, CUBE_SIZE);
			if obj = FILLED then
				Color(RED);

			CallObj(FACE);
		PopMatrix;

		PushMatrix;
			Translate(CUBE_SIZE, 0.0, 0.0);
			Rotate(90.0, 'Y');
			if obj = FILLED then
				Color(GREEN);

			CallObj(FACE);
		PopMatrix;

		PushMatrix;
			Translate(0.0, 0.0, -CUBE_SIZE);
			Rotate(180.0, 'y');
			if obj = FILLED  then
				Color(BLUE);

			CallObj(FACE);
		PopMatrix;

		PushMatrix;
			Translate(-CUBE_SIZE, 0.0, 0.0);
			Rotate(-90.0, 'y');
			if obj = FILLED then
				Color(CYAN);

			CallObj(FACE);
		PopMatrix;

		PushMatrix;
			Translate(0.0, CUBE_SIZE, 0.0);
			Rotate(-90.0, 'x');
			if obj = FILLED  then
				Color(MAGENTA);

			CallObj(FACE);
		PopMatrix;

		PushMatrix;
			Translate(0.0, -CUBE_SIZE, 0.0);
			Rotate(90.0, 'x');
			if obj = FILLED then
				Color(YELLOW);

			CallObj(FACE);
		PopMatrix;

		if obj = OUTLINE then
			PopAttributes;
		
	CloseObj
	end;

begin
	tdir := TRANS;
	scal := 1.0 + SCAL;

	write('Enter output device: ');
	readln(device);

	PrefPosition(50, 50);
	PrefSize(500, 500);

	Vinit(device);

	Window(-800.0, 800.0, -800.0, 800.0, -800.0, 800.0);
	LookAt(0.0, 0.0, 1500.0, 0.0, 0.0, 0.0, 0.0);

	(*
	 * Start with a very ordinary filled cube like the old demo...
	 *)

	PolyHatch(false);
	HatchAng(45.0);
	HatchPitch(40.0);
	PolyFill(true);

	fill := true;
	hatch := false;
	back := true;


	MakeObj(FACE);
		MakePoly;
			Rect(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
		ClosePoly;
	CloseObj;

	makecube(FILLED);

	nplanes := GetDepth;

	if nplanes = 1 then
		makecube(OUTLINE);

	BackFace(true);
		
	if BackBuffer < 0 then begin
		VgetDev(device);
		Vexit;
		writeln('lcube: device ', device, ' doesn''t support double buffering.'); 
		halt
	end;     

	but := 0;
	while  but <> 44 do begin (* Basically forever *)
		but := Slocator(x, y);
		PushMatrix;
			Rotate(100.0 * x, 'y');
			Rotate(100.0 * y, 'x');
			Color(BLACK);
			Clear;
			CallObj(FILLED);
			if ((nplanes = 1) and (fill or hatch)) then
				CallObj(OUTLINE);
		PopMatrix;
		SwapBuffers;

		but := CheckKey;
		case but of
		ord('p'):
			begin
				Voutput('lcube.ps');
				VnewDev('postscript');
				PushMatrix;
					Rotate(100.0 * x, 'y');
					Rotate(100.0 * y, 'x');
					Color(BLACK);
					Clear;
					CallObj(FILLED);
					if ((nplanes = 1) and (fill or hatch)) then
						CallObj(OUTLINE);
				PopMatrix;
				VnewDev(device);
			end;
		ord('f'):		(* Toggle filling *)
			begin
				fill := not fill;
				hatch := false;
				PolyFill(fill)
			end;
		ord('h'):		(* Toggle hatching *)
			begin
				hatch := not hatch;
				fill := false;
				PolyHatch(hatch)
			end;
		ord('b'):
			begin
				back := not back;
				BackFace(back)
			end;
		ord('s'):
			Scale(scal, scal, scal);
		ord('x'):
			Translate(tdir, 0.0, 0.0);
		ord('y'):
			Translate(0.0, tdir, 0.0);
		ord('z'):
			Translate(0.0, 0.0, tdir);
		ord('-'):
			begin
				tdir := -tdir;
				if scal < 1.0 then
					scal := 1.0 + SCAL
				else
					scal := 1.0 - SCAL

			end;
		ord('+'):
			tdir := TRANS;
		27: begin (* ESC *)
				Vexit;
				halt;
			end;
		ord('q'): begin
				Vexit;
				halt;
			end;
		otherwise
		end;
	end;

	Vexit;
end.

