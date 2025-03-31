
program patches;

#include 'Vogle.h'

(*
 *	Draws patches of various bases
 *)

var	basis: array[1..4] of Matrix44_t;
	x1, y1, z1, x2, y2, z2: Matrix44_t;

	labels: array[1..4] of string_t;

	dev: string_t;
	dum, i: integer;
	

	procedure init;
	begin
		(* 
		 * Gee, I really really really love Pascal sometimes....
		 *)
		(*
		 * patch basis types
		 *)

		(* Bezier *)
		basis[1, 1, 1] := -1.0;
		basis[1, 1, 2] :=  3.0;
		basis[1, 1, 3] := -3.0;
		basis[1, 1, 4] :=  1.0;
		basis[1, 2, 1] :=  3.0; 
		basis[1, 2, 2] := -6.0;
		basis[1, 2, 3] :=  3.0;
		basis[1, 2, 4] :=  0.0;
		basis[1, 3, 1] := -3.0;
		basis[1, 3, 2] :=  3.0;
		basis[1, 3, 3] :=  0.0;
		basis[1, 3, 4] :=  0.0;
		basis[1, 4, 1] :=  1.0;
		basis[1, 4, 2] :=  0.0;
		basis[1, 4, 3] :=  0.0;
		basis[1, 4, 4] :=  0.0;

		(* Cardinal basis *)
		basis[2, 1, 1] := -0.5;
		basis[2, 1, 2] :=  1.5;
		basis[2, 1, 3] := -1.5;
		basis[2, 1, 4] :=  0.5;
		basis[2, 2, 1] :=  1.0;
		basis[2, 2, 2] := -2.5;
		basis[2, 2, 3] :=  2.0;
		basis[2, 2, 4] := -0.5;
		basis[2, 3, 1] := -0.5;
		basis[2, 3, 2] :=  0.0;
		basis[2, 3, 3] :=  0.5;
		basis[2, 3, 4] :=  0.0;
		basis[2, 4, 1] :=  0.0;
		basis[2, 4, 2] :=  1.0;
		basis[2, 4, 3] :=  0.0;
		basis[2, 4, 4] :=  0.0;

		
		(* Bspline basis *)
		basis[3, 1, 1] := -0.166666;
		basis[3, 1, 2] :=  0.5;
		basis[3, 1, 3] := -0.5;
		basis[3, 1, 4] :=  0.166666;
		basis[3, 2, 1] :=  0.5;
		basis[3, 2, 2] := -1.0;
		basis[3, 2, 3] :=  0.5;
		basis[3, 2, 4] :=  0.0;
		basis[3, 3, 1] := -0.5;
		basis[3, 3, 2] :=  0.0;
		basis[3, 3, 3] :=  0.5;
		basis[3, 3, 4] :=  0.0;
		basis[3, 4, 1] :=  0.166666;
		basis[3, 4, 2] :=  0.666666;
		basis[3, 4, 3] :=  0.166666;
		basis[3, 4, 4] :=  0.0;

		(* 'Power' basis *)
		basis[4, 1, 1] := 1.0;
		basis[4, 1, 2] := 0.0;
		basis[4, 1, 3] := 0.0;
		basis[4, 1, 4] := 0.0;
		basis[4, 2, 1] := 0.0;
		basis[4, 2, 2] := 1.0;
		basis[4, 2, 3] := 0.0;
		basis[4, 2, 4] := 0.0;
		basis[4, 3, 1] := 0.0;
		basis[4, 3, 2] := 0.0;
		basis[4, 3, 3] := 1.0;
		basis[4, 3, 4] := 0.0;
		basis[4, 4, 1] := 0.0;
		basis[4, 4, 2] := 0.0;
		basis[4, 4, 3] := 0.0;
		basis[4, 4, 4] := 1.0;

		(*
		 * Geometry matricies
		 *)
		x1[1, 1] :=  0.00;
		x1[2, 1] :=  0.00;
		x1[3, 1] :=  0.00;
		x1[4, 1] :=  0.00;
		x1[1, 2] :=  0.26;
		x1[2, 2] :=  0.52;
		x1[3, 2] :=  0.52;
		x1[4, 2] :=  0.26;
		x1[1, 3] :=  0.50;
		x1[2, 3] :=  1.00;
		x1[3, 3] :=  1.00;
		x1[4, 3] :=  0.50;
		x1[1, 4] :=  0.71;
		x1[2, 4] :=  1.41;
		x1[3, 4] :=  1.41;
		x1[4, 4] :=  0.71;

		y1[1, 1] :=  1.00;
		y1[2, 1] :=  2.00;
		y1[3, 1] :=  2.00;
		y1[4, 1] :=  1.00;
		y1[1, 2] :=  0.97;
		y1[2, 2] :=  1.93;
		y1[3, 2] :=  1.93;
		y1[4, 2] :=  0.97;
		y1[1, 3] :=  0.87;
		y1[2, 3] :=  1.73;
		y1[3, 3] :=  1.73;
		y1[4, 3] :=  0.87;
		y1[1, 4] :=  0.71;
		y1[2, 4] :=  1.41;
		y1[3, 4] :=  1.41;
		y1[4, 4] :=  0.71;

		z1[1, 1] :=  1.00;
		z1[2, 1] :=  1.00;
		z1[3, 1] :=  0.00;
		z1[4, 1] :=  0.00;
		z1[1, 2] :=  1.00;
		z1[2, 2] :=  1.00;
		z1[3, 2] :=  0.00;
		z1[4, 2] :=  0.00;
		z1[1, 3] :=  1.00;
		z1[2, 3] :=  1.00;
		z1[3, 3] :=  0.00;
		z1[4, 3] :=  0.00;
		z1[1, 4] :=  1.00;
		z1[2, 4] :=  1.00;
		z1[3, 4] :=  0.00;
		z1[4, 4] :=  0.00;

		x2[1, 1] :=  0.71;
		x2[2, 1] :=  1.41;
		x2[3, 1] :=  1.41;
		x2[4, 1] :=  0.71;
		x2[1, 2] :=  0.87;
		x2[2, 2] :=  1.73;
		x2[3, 2] :=  1.73;
		x2[4, 2] :=  0.87;
		x2[1, 3] :=  0.97;
		x2[2, 3] :=  1.93;
		x2[3, 3] :=  1.93;
		x2[4, 3] :=  0.97;
		x2[1, 4] :=  1.00;
		x2[2, 4] :=  2.00;
		x2[3, 4] :=  2.00;
		x2[4, 4] :=  1.00;

		y2[1, 1] :=  0.71;
		y2[2, 1] :=  1.41;
		y2[3, 1] :=  1.41;
		y2[4, 1] :=  0.71;
		y2[1, 2] :=  0.50;
		y2[2, 2] :=  1.00;
		y2[3, 2] :=  1.00;
		y2[4, 2] :=  0.50;
		y2[1, 3] :=  0.26;
		y2[2, 3] :=  0.52;
		y2[3, 3] :=  0.52;
		y2[4, 3] :=  0.26;
		y2[1, 4] :=  0.00;
		y2[2, 4] :=  0.00;
		y2[3, 4] :=  0.00;
		y2[4, 4] :=  0.00;

		z2[1, 1] :=  1.00;
		z2[2, 1] :=  1.00;
		z2[3, 1] :=  0.00;
		z2[4, 1] :=  0.00;
		z2[1, 2] :=  1.00;
		z2[2, 2] :=  1.00;
		z2[3, 2] :=  0.00;
		z2[4, 2] :=  0.00;
		z2[1, 3] :=  1.00;
		z2[2, 3] :=  1.00;
		z2[3, 3] :=  0.00;
		z2[4, 3] :=  0.00;
		z2[1, 4] :=  1.00;
		z2[2, 4] :=  1.00;
		z2[3, 4] :=  0.00;
		z2[4, 4] :=  0.00;

		labels[1] := 'Bezier Patch(es)';
		labels[2] := 'Cardinal Patch(es)';
		labels[3] := 'B-Spline Patch(es)';
		labels[4] := '''Power'' Patch(es)'
	end;

	(*
	 * axes
	 *
	 *	draw the axes
	 *)

	procedure axes;
	begin
		Color(YELLOW);
		Move(0.0, 0.0, 0.0);
		Draw(4.0, 0.0, 0.0);

		Move(0.0, 0.0, 0.0);
		Draw(0.0, 4.0, 0.0);

		Move(0.0, 0.0, 0.0);
		Draw(0.0, 0.0, 4.0)
	end;

	(*
	 * drawhull
	 *
	 *	draw the hull for x, y, and z.
	 *)
	procedure drawhull(x, y, z: Matrix44_t);
		var i, j: integer;
	begin

		Color(MAGENTA);	

		for i := 1 to 4 do begin
			Move(x[i, 1], y[i, 1], z[i, 1]);
			for j := 1 to 4 do
				Draw(x[i, j], y[i, j], z[i, j])
		end;

		for i := 1 to 4 do begin
			Move(x[1, i], y[1, i], z[1, i]);
			for j := 1 to 4 do
				Draw(x[j, i], y[j, i], z[j, i])
		end;

		(* 
		 * Set color for The patch
		 *)
		Color(GREEN)
	end;


begin
	(*
	 * demonstrate patches
	 *)

	init;

	write('Enter device: ');
	readln(dev);

	Vinit(dev);
	VsetFlush(false);

	Color(BLACK);
	Clear;

	(*
	 * Set up two viewports (They actually overlap)
	 *)

	ViewPort(-1.0, 0.3, -1.0, 0.3);
	Ortho(-2.0, 5.0, -2.0, 5.0, -2.0, 5.0);
	LookAt(0.0, 0.0, 0.0, -3.0, 2.0, -4.0, 0.0);
	(*
	 * Save it 
	 *)
	PushViewPort;
	PushMatrix;

	ViewPort(-0.3, 1.0, -0.3, 1.0);
	Ortho(-2.0, 5.0, -2.0, 5.0, -2.0, 5.0);
	LookAt(0.0, 0.0, 0.0, 3.0, 2.0, -4.0, 0.0);

	TextSize(0.4, 0.4);

	(*
	 * patchcurves provides a number of curves in the t and u
	 * directions. patchprecision gives the minimum number of line
	 * segments making up the curves in the t and u directions. The
	 * actual number of linesegments in t or u is equal to the closest
	 * integer multiple of the number of curves, > nsegs, in t or u,
	 * greater than or equal to the number set by patchprecision in u or
	 * t. eg. curves in t will be made up of 21 line segments so that we
	 * can match up the 7 curves in u; curves in u will have 24 as 4 by 5
	 * gives 20.
	 *)
	PatchCurves(4, 7);
	PatchPrecision(20, 20);

	for i := 1 to 4 do
	begin

		axes;


		(*
		 * patchbasis sets the basis matrices for the t and u
		 * functions
		 * 
		 *)
		PatchBasis(basis[i], basis[i]);

		(* 
		 * Draw with viewport 2
		 *)
		Move(0.0, 4.0, 0.0);
		DrawStr(labels[i]);

		(*
		 * now draw the patches according to the geometry matrices in
		 * x1, y1, and z1, x2, y2, z2.
		 *)
		drawhull(x1, y1, z1);
		Patch(x1, y1, z1);

		drawhull(x2, y2, z2);
		Patch(x2, y2, z2);

		(*
		 * Now with viewport 1
		 *)
		PopViewPort;
		PopMatrix;

		axes;

		Move(0.0, 4.0, 0.0);
		DrawStr(labels[i]);

		(*
		 * now draw the patches according to the geometry matrices in
		 * x1, y1, and z1, x2, y2, z2.
		 *)
		drawhull(x1, y1, z1);
		Patch(x1, y1, z1);

		drawhull(x2, y2, z2);
		Patch(x2, y2, z2);

		dum := GetKey;

		(*
		 * Save viewport 1 again and reset to viewport 2
		 *)
		PushViewPort;
		PushMatrix;

		ViewPort(-0.3, 1.0, -0.3, 1.0);
		Ortho(-1.5, 5.0, -1.5, 5.0, -1.5, 5.0);
		LookAt(0.0, 0.0, 0.0, 3.0, 2.0, -4.0, 0.0);

		Color(BLACK);
		Clear
	end;

	Vexit
end.

