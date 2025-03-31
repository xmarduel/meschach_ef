program circtxt;
(*
 * display all the hershey fonts and demonstrate textang
 *)

#include 'Vogle.h'

const
	pi = 3.14159265358979;

var
	fonts: array [1..22] of string_t;
	str1, str2, str3, str4, buf, device: string_t;
	dum, i: integer;
		

	procedure init;
	begin
		str1 := 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
		str2 := 'abcdefghijklmnopqrstuvwxyz';
		str3 := '1234567890+-=!@#$%^&*(){}[]';
		str4 := '<>,./?~`\\|_BONK,blark';

		fonts[1] := 'astrology';
		fonts[2] := 'cursive';
		fonts[3] := 'futura.l';
		fonts[4] := 'futura.m';
		fonts[5] := 'gothic.eng';
		fonts[6] := 'gothic.ger';
		fonts[7] := 'gothic.ita';
		fonts[8] := 'greek';
		fonts[9] := 'japanese';
		fonts[10] := 'markers';
		fonts[11] := 'math.low';
		fonts[12] := 'math.upp';
		fonts[13] := 'meteorology';
		fonts[14] := 'music';
		fonts[15] := 'cyrillic';
		fonts[16] := 'script';
		fonts[17] := 'symbolic';
		fonts[18] := 'times.g';
		fonts[19] := 'times.ib';
		fonts[20] := 'times.i';
		fonts[21] := 'times.r';
		fonts[22] := 'times.rb'
	end;

	procedure ShowCircularText(r: real; str: string_t);
	(*
	 * ShowCircularText
	 *
	 *	show a ring of text
	 *)
	var
		a, j, inc, x, y: real;
		slen, c, ii: integer;

	begin

		slen := length(str);
		inc := 360.0 / slen;

		c := 1;
		j := 0.0;

		for ii := 1 to slen do
		begin

			j := j + inc;
			(*
			 * calculate the next drawing position
			 *)
			x := r * cos(j * pi / 180.0);
			y := r * sin(j * pi / 180.0);
			Move2(x, y);
			(*
			 * calculate angle for next character
			 *)
			a := (90 + j);
			(*
			 * set the orientation of the next character
			 *)
			TextAng(a);
			(*
			 * draw the character
			 *)
			DrawChar(str[c]);
			c := c + 1
		end
	end;

begin

	init;

	write('Enter device name: ');
	readln(buf);

	Vinit(buf);
	VsetFlush(false);

	Color(BLACK);
	Clear;

	Ortho2(-14.0, 14.0, -14.0, 14.0);	(* define the world space *)

	for i := 1 to 22 do
	begin

		(*
		 * textang is used to specify the orientation of text. As
		 * we want the title to come out straight we make sure it is
		 * zero each time we go through this loop.
		 *)
		TextAng(0.0);

		(*
		 * do the title
		 *)
		Color(YELLOW);
		Font('futura.m');
		buf := 'This is hershey font ' + fonts[i];
		BoxText(-11.0, 12.0, 20.0, 1.0, buf);

		(*
		 * draw a box around the title
		 *)
		Rect(-11.0, 12.0, 9.0, 13.0);

		Color(GREEN);

		writeln('font: ', fonts[i]);
		Font(fonts[i]);		(* grab a font from the table *)

		TextSize(1.5, 1.5);		(* show the outer ring *)
		ShowCircularText(11.0, str1);

		TextSize(1.3, 1.3);		(* show the second ring *)
		ShowCircularText(8.5, str2);

		TextSize(1.1, 1.1);		(* show the third ring *)
		ShowCircularText(7.0, str3);

		TextSize(0.9, 0.9);		(* show the inside ring *)
		ShowCircularText(5.0, str4);

		dum := GetKey;

		if dum = ord('q') then halt;

		Color(BLACK);
		Clear
	end;

	Vexit
end.

