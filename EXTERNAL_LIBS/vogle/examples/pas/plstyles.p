program lstyles;

#include "Vogle.h"

var
	idum: integer;
	device: string_t;

procedure drawbox(s: real);
begin
	PushMatrix;

	Rotate(30.0, 'x');
	Rotate(60.0, 'y');
	Translate(-0.7, -1.2, 0.0);
	Scale(s, s, s);

	Move(0.0, 0.0, 0.0);

	Draw(1.0, 0.0, 0.0);
	Draw(1.0, 1.0, 0.0);
	Draw(0.0, 1.0, 0.0);
	Draw(0.0, 0.0, 0.0);

	Draw(0.0, 0.0, -1.0);
	Draw(1.0, 0.0, -1.0);
	Draw(1.0, 1.0, -1.0);
	Draw(0.0, 1.0, -1.0);
	Draw(0.0, 0.0, -1.0);

	Move(0.0, 1.0, -1.0);
	Draw(0.0, 1.0, 0.0);

	Move(1.0, 1.0, 0.0);
	Draw(1.0, 1.0, -1.0);

	Move(1.0, 0.0, 0.0);
	Draw(1.0, 0.0, -1.0);

	PopMatrix;
end;


procedure drawsine(s: real);

const
	RAD = 0.5;
	AMP = 0.03;
var
	a, x, y, z: real;

begin

	PushMatrix;

	Translate(RAD + 0.2, -0.5, 0.0);
	Scale(s, s, s);

	Move(RAD, 0.0, 0.0);
	a := 0;
	while (a <= 2 * 3.1415926) do begin
		x := RAD * cos(a);
		z := RAD * sin(a);
		y := AMP * sin(a * 6.0);

		Draw(x, y, z);

		a := a + 0.02
	end;

	PopMatrix
end;

procedure drawscene;
begin
	Color(BLACK);
	Clear;

	Color(GREEN);
	SetDash(0.03);

	XcenterText;
	Move2(-0.45, 0.9);
	DrawStr('Linestyle: "10"');
	Move2(-0.45, 0.7);
	DrawStr('Linestyle: "110"');
	Move2(-0.45, 0.5);
	DrawStr('Linestyle: "111010"');
	Move2(-0.45, 0.3);
	DrawStr('Linestyle: "0001"');

	LineStyle('10');
	Move2(-0.9, 0.9);
	Draw2( 0.0, 0.9);
	Circle(0.6, 0.6, 0.4);

	drawbox(0.9);
	drawsine(0.9);

	Color(RED);
	LineStyle('110');
	Move2(-0.9, 0.7);
	Draw2( 0.0, 0.7);
	Circle(0.6, 0.6, 0.3);
	drawbox(0.7);
	drawsine(0.7);

	Color(CYAN);
	LineStyle('111010');
	Move2(-0.9, 0.5);
	Draw2( 0.0, 0.5);
	Circle(0.6, 0.6, 0.2);
	drawbox(0.5);
	drawsine(0.5);

	Color(YELLOW);
	LineStyle('0001');
	Move2(-0.9, 0.3);
	Draw2( 0.0, 0.3);
	Circle(0.6, 0.6, 0.1);
	drawbox(0.3);
	drawsine(0.3);

	idum := GetKey;	

end;


begin
	write('Enter output device: ');
	readln(device);

	Vinit(device);
	VsetFlush(false);
	Up(0.0, 1.0, 0.0);
	Perspective(90.0, 1.0, 0.3, 3.0);
	Translate(0.0, 0.0, -1.3);

	drawscene;
	Rotate(-30.0, 'y');
	Rotate(-30.0, 'x');
	drawscene;

	Vexit
end.
