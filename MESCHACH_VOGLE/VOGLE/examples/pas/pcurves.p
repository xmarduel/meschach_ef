program curves;
(*
 * using curves
 *)

#include 'Vogle.h'

var	bezier, cardinal, bspline: Matrix44_t;
	geom1: Matrix43_t;
	geom2: GeomMat_t;
	dev: string_t;
	c: char;
	i: integer;

	procedure init;
	begin
		(*
		 * curve basis types
		 *)
		bezier[1, 1] := -1.0;
		bezier[1, 2] :=  3.0;
		bezier[1, 3] := -3.0;
		bezier[1, 4] :=  1.0;
		bezier[2, 1] :=  3.0; 
		bezier[2, 2] := -6.0;
		bezier[2, 3] :=  3.0;
		bezier[2, 4] :=  0.0;
		bezier[3, 1] := -3.0;
		bezier[3, 2] :=  3.0;
		bezier[3, 3] :=  0.0;
		bezier[3, 4] :=  0.0;
		bezier[4, 1] :=  1.0;
		bezier[4, 2] :=  0.0;
		bezier[4, 3] :=  0.0;
		bezier[4, 4] :=  0.0;

		cardinal[1, 1] := -0.5;
		cardinal[1, 2] :=  1.5;
		cardinal[1, 3] := -1.5;
		cardinal[1, 4] :=  0.5;
		cardinal[2, 1] :=  1.0;
		cardinal[2, 2] := -2.5;
		cardinal[2, 3] :=  2.0;
		cardinal[2, 4] := -0.5;
		cardinal[3, 1] := -0.5;
		cardinal[3, 2] :=  0.0;
		cardinal[3, 3] :=  0.5;
		cardinal[3, 4] :=  0.0;
		cardinal[4, 1] :=  0.0;
		cardinal[4, 2] :=  1.0;
		cardinal[4, 3] :=  0.0;
		cardinal[4, 4] :=  0.0;

		
		bspline[1, 1] := -0.166666;
		bspline[1, 2] :=  0.5;
		bspline[1, 3] := -0.5;
		bspline[1, 4] :=  0.166666;
		bspline[2, 1] :=  0.5;
		bspline[2, 2] := -1.0;
		bspline[2, 3] :=  0.5;
		bspline[2, 4] :=  0.0;
		bspline[3, 1] := -0.5;
		bspline[3, 2] :=  0.0;
		bspline[3, 3] :=  0.5;
		bspline[3, 4] :=  0.0;
		bspline[4, 1] :=  0.166666;
		bspline[4, 2] :=  0.666666;
		bspline[4, 3] :=  0.166666;
		bspline[4, 4] :=  0.0;

		(*
		 * 	Geometry matrix to demonstrate basic spline segments
		 *)

		geom1[1, 1] := -180.0;
		geom1[1, 2] :=   10.0;
		geom1[1, 3] :=    0.0;
		geom1[2, 1] := -100.0;
		geom1[2, 2] :=  110.0;
		geom1[2, 3] :=    0.0;
		geom1[3, 1] := -100.0;
		geom1[3, 2] :=  -90.0;
		geom1[3, 3] :=    0.0;
		geom1[4, 1] :=    0.0;
		geom1[4, 2] :=   50.0;
		geom1[4, 3] :=    0.0;
		(*
		 * 	Geometry matrix to demonstrate overlapping control 
		 *	points to produce continuous (Well, except for the
		 * 	bezier ones) curves from spline segments.
		 *)

		geom2[1, 1] := 200.0;
		geom2[1, 2] := 480.0;
		geom2[1, 3] :=   0.0;
		geom2[2, 1] := 380.0;
		geom2[2, 2] := 180.0;
		geom2[2, 3] :=   0.0;
		geom2[3, 1] := 250.0;
		geom2[3, 2] := 430.0;
		geom2[3, 3] :=   0.0;
		geom2[4, 1] := 100.0;
		geom2[4, 2] := 130.0;
		geom2[4, 3] :=   0.0;
		geom2[5, 1] :=  50.0;
		geom2[5, 2] := 280.0;
		geom2[5, 3] :=   0.0;
		geom2[6, 1] := 150.0;
		geom2[6, 2] := 380.0;
		geom2[6, 3] :=   0.0
	end;

begin
	init;

	write('Enter device: ');
	readln(dev);

	Vinit(dev);

	Ortho2(-200.0, 400.0, -100.0, 500.0);

	Color(BLACK);
	Clear;

	Color(YELLOW);

	TextSize(10.0, 10.0);

	(*
	 * label the control points in geom1
	 *)
	for i := 1 to 4 do begin
		Move2(geom1[i, 1], geom1[i, 2]);
		c := chr(ord('0') + i);
		DrawChar(c)
	end;
								 
	(*
	 * label the control points in geom2
	 *)
	for i := 1 to 6 do begin
		Move2(geom2[i, 1], geom1[i, 2]);
		c := chr(ord('0') + i);
		DrawChar(c)

	end;

	(*
	 * scale the current font so that 30 of the largest characters
	 * in the current font will fit in a region 300 world units wide,
	 * 20 high.
	 *)
	BoxFit(300.0, 20.0, 30);

	(*
	 * set the number of line segments appearing in each curve to 20
	 *)
	CurvePrecision(20);

	(*
	 * copy the bezier basis matrix into the curve basis matrix.
	 *)
	CurveBasis(bezier);

	Color(RED);

	(*
	 * draw a curve using the current basis matrix (bezier in this case)
	 * and the control points in geom1
	 *)
	Curve(geom1);

	Move2(70.0, 60.0);
	DrawStr('Bezier Curve Segment');

	Move2(-190.0, 450.0);
	DrawStr('Three overlapping Bezier Curves');

	(*
	 * curven draws overlapping curve segments according to geom2, the
	 * number of curve segments drawn is three less than the number of
	 * points passed, assuming there are a least four points in the
	 * geometry matrix (in this case geom2). This call will draw 3
	 * overlapping curve segments in the current basis matrix - still
	 * bezier.
	 *)
	Curven(6, geom2);

	i := GetKey;

	(*
	 * load in the cardinal basis matrix
	 *)
	CurveBasis(cardinal);

	Color(MAGENTA);

	Move2(70.0, 10.0);
	DrawStr('Cardinal Curve Segment');

	(*
	 * plot out a curve segment using the cardinal basis matrix
	 *)
	Curve(geom1);

	Move2(-190.0, 400.0);
	DrawStr('Three overlapping Cardinal Curves');

	(*
	 * now draw a bunch of them again.
	 *)
	Curven(6, geom2);

	i := GetKey;

	(*
	 * change the basis matrix again
	 *)
	CurveBasis(bspline);

	Color(GREEN);

	Move2(70.0, -40.0);
	DrawStr('Bspline Curve Segment');

	(*
	 * now draw our curve segment in the new basis...
	 *)
	Curve(geom1);

	Move2(-190.0, 350.0);
	DrawStr('Three overlapping Bspline Curves');

	(*
	 * ...and do some overlapping ones
	 *)
	Curven(6, geom2);

	i := GetKey;

	Vexit
end.
