program pjtext;


#include "Vogle.h"

var
        dev: string_t;
	dum: integer;

procedure drawstuff;
begin
	Color(BLACK);
	PolyFill(true);	(* So rect clears a bit for us *)
	Rect(0.1, 0.1, 0.9, 0.9);
	Color(WHITE);
	Move2(0.1, 0.5);
	Draw2(0.9, 0.5);
	Move2(0.5, 0.1);
	Draw2(0.5, 0.9);

	Color(GREEN);
	Move2(0.5, 0.5);
	LeftJustify();
	DrawStr('This is Left Justified text');

	dum := GetKey();

	Color(BLACK);
	Rect(0.1, 0.1, 0.9, 0.9);
	Color(WHITE);
	Move2(0.1, 0.5);
	Draw2(0.9, 0.5);
	Move2(0.5, 0.1);
	Draw2(0.5, 0.9);

	Color(YELLOW);
	Move2(0.5, 0.5);
	CenterText(true);
	DrawStr('This is Centered text');
	CenterText(false);

	dum := GetKey();

	Color(BLACK);
	Rect(0.1, 0.1, 0.9, 0.9);
	Color(WHITE);
	Move2(0.1, 0.5);
	Draw2(0.9, 0.5);
	Move2(0.5, 0.1);
	Draw2(0.5, 0.9);

	Color(MAGENTA);
	Move2(0.5, 0.5);
	RightJustify();
	DrawStr('This is Right Justified text');
	TextJustify(0);

	dum := GetKey()
end;

procedure drawstuff2(ang : real);
begin
 	Color(BLACK);
	Rect(0.1, 0.1, 0.9, 0.9);
	Color(WHITE);
	Move2(0.1, 0.5);
	Draw2(0.9, 0.5);
	Move2(0.5, 0.1);
	Draw2(0.5, 0.9);

	TextAng(ang);

	Color(GREEN);
	Move2(0.5, 0.5);
	LeftJustify();
	DrawChar('B');

	TextAng(0.0);
	TextJustify(0);
	BoxText(0.1, 0.1, 0.4, 0.02, 'The ''B'' should be leftjustified');
	
	dum := GetKey();

	Color(BLACK);
	Rect(0.1, 0.1, 0.9, 0.9);
	Color(WHITE);
	Move2(0.1, 0.5);
	Draw2(0.9, 0.5);
	Move2(0.5, 0.1);
	Draw2(0.5, 0.9);

	TextAng(ang);
	Color(YELLOW);
	Move2(0.5, 0.5);
	CenterText(true);
	DrawChar('B');
	CenterText(false);

	TextAng(0.0);
	TextJustify(0);
	BoxText(0.1, 0.1, 0.4, 0.02, 'The ''B'' should be centered');

	dum := GetKey();

	Color(BLACK);
	Rect(0.1, 0.1, 0.9, 0.9);
	Color(WHITE);
	Move2(0.1, 0.5);
	Draw2(0.9, 0.5);
	Move2(0.5, 0.1);
	Draw2(0.5, 0.9);

	TextAng(ang);
	Color(MAGENTA);
	Move2(0.5, 0.5);
	RightJustify();
	DrawChar('B');

	TextAng(0.0);
	TextJustify(0);
	BoxText(0.1, 0.1, 0.4, 0.02, 'The ''B'' should be rightjustified');

	dum := GetKey();

	Color(BLACK);
	Rect(0.1, 0.1, 0.9, 0.9);
	Color(WHITE);
	Move2(0.1, 0.5);
	Draw2(0.9, 0.5);
	Move2(0.5, 0.1);
	Draw2(0.5, 0.9);

	TextAng(ang);
	Color(MAGENTA);
	Move2(0.5, 0.5);
	TopJustify();
	DrawChar('B');

	TextAng(0.0);
	TextJustify(0);
	BoxText(0.1, 0.1, 0.4, 0.02, 'The ''B'' should be topjustified');

	dum := GetKey();

	Color(BLACK);
	Rect(0.1, 0.1, 0.9, 0.9);
	Color(WHITE);
	Move2(0.1, 0.5);
	Draw2(0.9, 0.5);
	Move2(0.5, 0.1);
	Draw2(0.5, 0.9);

	TextAng(ang);
	Color(MAGENTA);
	Move2(0.5, 0.5);
	TopJustify();
	RightJustify();
	DrawChar('B');

	TextAng(0.0);
	TextJustify(0);
	BoxText(0.1, 0.1, 0.4, 0.02, 'The ''B'' should be right/topjustified');

	TextJustify(0);

	dum := GetKey()
end;

begin
	write('Enter device: ');
	readln(dev);
	Vinit(dev);

	TextSize(0.03, 0.04);

	Ortho2(0.0, 1.0, 0.0, 1.0);

	Color(RED);
	Clear();

	drawstuff();

	(* Now do it all with the text rotated .... *)

	TextAng(45.0);
	drawstuff();

	TextAng(160.0);
	drawstuff();

	TextAng(270.0);
	drawstuff();

	(* Now with a single character *)

	TextJustify(0);

	drawstuff2(0.0);

	drawstuff2(90.0);

	drawstuff2(160.0);

	drawstuff2(270.0);

	Vexit()

end.
