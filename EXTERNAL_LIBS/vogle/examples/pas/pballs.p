program balls;

#include 'Vogle.h'

const
	RADIUS = 10.0;
	SPHERE = 1;
	PI     = 3.14159265358979;
var
	dev: string_t;
	dum: integer;

	(*
	 * makesphere
	 *
	 *	make a sphere object
	 *)
	procedure makesphere;

		var	ir, r, z, a: real;
			i: integer;
	begin

		MakeObj(SPHERE);

			(*
			 * create the latitudinal rings
			 *)
			for i := 0 to 9 do
			begin
				ir := i * 20.0;
				PushMatrix;
					Rotate(ir, 'y');
					Circle(0.0, 0.0, RADIUS);
				PopMatrix
			end;
			
			(*
			 * create the longitudinal rings
			 *)
			PushMatrix;
				Rotate(90.0, 'x');
				for i := 0 to 9 do
				begin
					a := -90.0 + i * 20.0;
					r := RADIUS * cos(a * PI / 180.0);
					z := RADIUS * sin(a * PI / 180.0);
					PushMatrix;
						Translate(0.0, 0.0, -z);
						Circle(0.0, 0.0, r);
					PopMatrix
				end;
			PopMatrix;

		CloseObj
	end;

(*
 * a demonstration of objects
 *)
begin

	write('Enter device: ');
	readln(dev);

	Vinit(dev);
	VsetFlush(false);

	(*
	 * set up our viewing transformation
	 *)
	Perspective(90.0, 1.0, 0.001, 500.0);
	LookAt(13.0, 13.0, 8.0, 0.0, 0.0, 0.0, 0.0);

	Color(BLACK);
	Clear;

	(*
	 * Call a routine to make the sphere object
	 *)
	makesphere;

	(*
	 * Now draw the sphere object scaled down. We use the pushmatrix
	 * and the popmatrix to preserve the transformation matrix so
	 * that only this sphere is drawn scaled.
	 *)
	Color(CYAN);

	PushMatrix;
		Scale(0.5, 0.5, 0.5);
		CallObj(SPHERE);
	PopMatrix;

	(*
	 * now we draw the same sphere translated, with a different
	 * scale and color.
	 *)

	Color(WHITE);

	PushMatrix;
		Translate(0.0, -1.4 * RADIUS, 1.4 * RADIUS);
		Scale(0.3, 0.3, 0.3);
		CallObj(SPHERE);
	PopMatrix;

	(*
	 * and maybe a few more times....
	 *)


	Color(RED);

	PushMatrix;
		Translate(0.0, RADIUS, 0.7 * RADIUS);
		Scale(0.2, 0.2, 0.2);
		CallObj(SPHERE);
	PopMatrix;

	Color(GREEN);

	PushMatrix;
		Translate(0.0, 1.5 * RADIUS, -RADIUS);
		Scale(0.15, 0.15, 0.15);
		CallObj(SPHERE);
	PopMatrix;

	Color(YELLOW);

	PushMatrix;
		Translate(0.0, -RADIUS, -RADIUS);
		Scale(0.12, 0.12, 0.12);
		CallObj(SPHERE);
	PopMatrix;

	Color(BLUE);

	PushMatrix;
		Translate(0.0, -2.0*RADIUS, -RADIUS);
		Scale(0.3, 0.3, 0.3);
		CallObj(SPHERE);
	PopMatrix;

	Font('times.rb');
	Ortho2(0.0, 1.0, 0.0, 1.0);
	CenterText(true);
	TextSize(0.08, 0.15);
	Move2(0.8, 0.5);
	TextAng(-90.0);
	DrawStr('I''m very ordinary!');

	dum := GetKey;

	Vexit
end.
