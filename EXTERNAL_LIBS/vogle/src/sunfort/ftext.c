#include <stdio.h>
#include <string.h>
#include "vogle.h"

/*
 * font_
 */
void
font_(char *fontfile, int len)
{
	char		buf[BUFSIZ];
	register char	*p;

	strncpy(buf, fontfile, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	font(buf);
}

/*
 * textsize_
 */
void
textsize_(float *width, float *height)
{
	textsize(*width, *height);
}

/*
 * boxtext_
 */
void
boxtext_(float *x, float *y, float *l, float *h, char *s, int len)
{
	char		buf[BUFSIZ];
	register char   *p;

	strncpy(buf, s, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	boxtext(*x, *y, *l, *h, buf);
}

/*
 * boxfit_
 */
void
boxfit_(float *l, float *h, int *nchars)
{
	boxfit(*l, *h, *nchars);
}

/*
 * textang_
 */
void
textang_(float *ang)
{
	textang(*ang);
}

/*
 * drawchar_
 */
void
drawchar_(char *s)
{
	drawchar(*s);
}

/*
 * drawstr_
 */
void
drawstr_(char *s, int len)
{
        char            buf[BUFSIZ];
	register char   *p;

	strncpy(buf, s, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	drawstr(buf);
}

/*
 * getfontsize_
 */
void
getfontsize_(float *cw, float *ch)
{
	getfontsize(cw, ch);
}

/*
 * getcharsize_
 */
void
getcharsize_(char *c, float *cw, float *ch)
{
	getcharsize(*c, cw, ch);
}

/*
 * fixedwidth
 */
void
fixedwidth_(int *i)
{
	fixedwidth(*i);
}

/*
 * centertext
 */
void
centertext_(int *i)
{
	int	j;
	j = (*i ? 1 : 0);
	centertext(j);
}

/*
 * textjustify
 */
void
textjustify_(int *i)
{
	textjustify((unsigned)*i);
}

/*
 * leftjustify
 */
void
leftjustify_(void)
{
	leftjustify();
}

/*
 * rightjustify
 */
void
rightjustify_(void)
{
	rightjustify();
}

/*
 * topjustify
 */
void
topjustify_(void)
{
	topjustify();
}

/*
 * bottomjustify
 */
void
bottomjustify_(void)
{
	bottomjustify();
}

/*
 * xcentertext
 */
void
xcentertext_(void)
{
	xcentertext();
}

/*
 * ycentertext
 */
void
ycentertext_(void)
{
	ycentertext();
}

/*
 * textslant
 */
void
textslant_(float *v)
{
	textslant(*v);
}

/*
 * textweight
 */
void
textweight_(int *v)
{
	textweight(*v);
}

/*
 * numchars_
 */
int
numchars_(void)
{
	return(numchars());
}

/*
 * strlength_
 */
float
strlength_(char *str, int len)
{
        char            buf[BUFSIZ];
	register char   *p;

	strncpy(buf, str, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	return(strlength(buf));
}
