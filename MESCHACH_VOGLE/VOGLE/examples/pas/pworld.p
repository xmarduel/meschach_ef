program world;

#include 'Vogle.h'

const
        RADIUS = 10.0;
        SPHERE = 1;
        PI     = 3.14159265358979;
var
        dev: string_t;
        i, dum: integer;
	r, z, a: real;


	(*
	 * MakeSphere
	 *
	 *	make a sphere object
	 *)
	procedure MakeSphere;

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
	 * showroundtext
	 *
	 *	draw string str wrapped around a circle in 3d
	 *)
	procedure showroundtext(str: string_t);
		var	i, inc: real;
			n: integer;
			c: char;
	begin
		inc := 360.0 / length(str);

		i := 0.0;
		n := 1;

		while (i < 360.0) do
		begin
			PushMatrix;
				(*
				 * find the spot on the edge of the sphere
				 * by making it (0, 0, 0) in world coordinates
				 *)
				Rotate(i, 'y');
				Translate(0.0, 0.0, RADIUS);

				Move(0.0, 0.0, 0.0);

				c := str[n];
				n := succ(n);
				DrawChar(c);
			PopMatrix;

			i := i + inc
		end
	end;

(*
 * most of the things in this program have been done before but it has
 * a certain novelty value.
 *)
begin

	write('Enter device: ');
	readln(dev);

	Vinit(dev);
	Clipping(false);
	VsetFlush(false);

	Font('futura.m');

	Perspective(80.0, 1.0, 0.001, 50.0);
	LookAt(13.0, 13.0, 8.0, 0.0, 0.0, 0.0, 0.0);

	Color(BLACK);
	Clear;

	MakeSphere;

	(*
	 * draw the main one in cyan
	 *)
	Color(CYAN);

	CallObj(SPHERE);

	(*
	 * draw a smaller one outside the main one in white
	 *)
	Color(WHITE);

	PushMatrix;
		Translate(0.0, -1.4 * RADIUS, 1.4 * RADIUS);
		Scale(0.3, 0.3, 0.3);
		CallObj(SPHERE);
	PopMatrix;

	(*
	 * scale the text
	 *)
	BoxFit(2.0 * PI * RADIUS, 0.25 * RADIUS, 31);

	(*
	 * now write the text in rings around the main sphere
	 *)

	Color(GREEN);
	showroundtext('Around the world in eighty days ');

	Color(BLUE);
	(*
	 * note: that software text is rotated here as
	 * anything else would be whether you use textang
	 * or rotate depends on what you are trying to do.
	 * Experience is the best teacher here.
	 *)
	Rotate(90.0, 'x');
	showroundtext('Around the world in eighty days ');

	Color(RED);
	Rotate(90.0, 'z');
	showroundtext('Around the world in eighty days ');

	dum := GetKey;

	Vexit
end.
