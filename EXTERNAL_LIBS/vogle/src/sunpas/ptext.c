#include <string.h>
#include <stdio.h>
#include "vogle.h"

/*
 * This should match the Pascal type 'varying [n] of char'
 */

typedef struct {
	int	n;
	char	s[1];	/* SO compiler doesn't bitch about zero length */
} Vstring;

#define COPYANDTERMINATE(buf, s, l)	strncpy(buf, s, l); buf[l] = '\0';

/*
 * Font
 */
void
Font(Vstring s)
{
	char	*p = (char *)s.s;
	char	buf[BUFSIZ];

	COPYANDTERMINATE(buf, p, s.n);
	font(buf);
}

/*
 * TextSize
 */
void
TextSize(double width, double height)
{
	textsize(width, height);
}

/*
 * BoxText
 */
void
BoxText(double x, double y, double l, double h, Vstring s)
{
	char	*p = (char *)s.s;
	char	buf[BUFSIZ];

	COPYANDTERMINATE(buf, p, s.n);
	boxtext(x, y, l, h, buf);
}

/*
 * BoxFit
 */
void
BoxFit(double l, double h, int nchars)
{
	boxfit(l, h, nchars);
}

/*
 * TextWeight
 */
void
TextWeight(int val)
{
	textweight(val);
}

/*
 * TextSlant
 */
void
TextSlant(double val)
{
	textslant(val);
}

/*
 * TextAng
 */
void
TextAng(double ang)
{
	textang(ang);
}

/*
 * DrawChar
 */
void
DrawChar(char s)
{
	drawchar(s);
}

/*
 * DrawStr
 */
void
DrawStr(Vstring s)
{
	char	*p = (char *)s.s;
	char	buf[BUFSIZ];

	COPYANDTERMINATE(buf, p, s.n);
	drawstr(buf);
}

/*
 * GetFontSize
 */
void
GetFontSize(double *cw, double *ch)
{
	float	w, h;
	getfontsize(&w, &h);

	*cw = w;
	*ch = h;
}

/*
 * GetCharSize
 */
void
GetCharSize(char c, double *cw, double *ch)
{
	float	w, h;
	getcharsize(c, &w, &h);

	*cw = w;
	*ch = h;
}

/*
 * FixedWidth
 */
void
FixedWidth(int i)
{
	fixedwidth(i);
}

/*
 * CenterText
 */
void
CenterText(int i)
{
	centertext(i);
}

/*
 * TextJustify
 */
void
TextJustify(int i)
{
	textjustify(i);
}

/*
 * LeftJustify
 */
void
LeftJustify(void)
{
	leftjustify();
}

/*
 * RightJustify
 */
void
RightJustify(void)
{
	rightjustify();
}

/*
 * TopJustify
 */
void
TopJustify(void)
{
	topjustify();
}

/*
 * BottomJustify
 */
void
BottomJustify(void)
{
	bottomjustify();
}

/*
 * XcenterText
 */
void
XcenterText(void)
{
	xcentertext();
}

/*
 * YcenterText
 */
void
YcenterText(void)
{
	ycentertext();
}

/*
 * NumChars
 */
int
NumChars(void)
{
	return(numchars());
}

/*
 * StrLength
 */
float
StrLength(Vstring s)
{
	char	*p = (char *)s.s;
	char	buf[BUFSIZ];

	COPYANDTERMINATE(buf, p, s.n);
	return(strlength(buf));
}
