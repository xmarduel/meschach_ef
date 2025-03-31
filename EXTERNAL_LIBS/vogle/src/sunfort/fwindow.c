#include <stdio.h>
#include <string.h>
#include "vogle.h"

/*
 * voutput_
 */
int
winopen_(const char *dev, const char *title, int lend, int lent)
{
	char		fdev[50], ftit[BUFSIZ];
	register char	*p;

	strncpy(fdev, dev, lend);
	fdev[lend] = 0;

	for (p = &fdev[lend - 1]; *p == ' '; p--)
		;

	*++p = 0;

	strncpy(ftit, title, lent);
	ftit[lent] = 0;

	for (p = &ftit[lent - 1]; *p == ' '; p--)
		;

	*++p = 0;

	return(winopen(fdev, ftit));

}

/*
 * winset_
 */
int
winset_(int *win)
{
	return(winset(*win));
}

/*
 * winclose_
 */
int
winclose_(int *win)
{
	return(winclose(*win));
}

/*
 * winraise_
 */
int
winraise_(int *win)
{
	return(winraise(*win));
}

/*
 * windel_
 */
int
windel_(int *win)
{
	return(windel(*win));
}


/*
 * vinit_
 */
int
vinit_(char *dev, int len)
{
	char		buf[BUFSIZ];
	register char	*p;

	strncpy(buf, dev, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	return(vinit(buf));
}

/*
 * vgetdev_
 */
void
vgetdev_(char *buf, int len)
{
	register int	p;

	(void)vgetdev(buf);

	for (p = strlen(buf); p < len; p++)
		buf[p] = ' ';
}

/*
 * vgetevent_
 *
 */
int
vgetevent_(int	vev[5], int *block)
{
	Vevent	ev;
	int	i;

	i = vgetevent(&ev, *block);
	vev[0] = ev.w;
	vev[1] = ev.type;
	vev[2] = ev.data;
	vev[3] = ev.x;
	vev[4] = ev.y;

	return(i);
}

/*
 * vgetwin_
 */
int
vgetwin_(void)
{
	return(vgetwin());
}

/*
 * vwinidvalid_
 */
int
vwinidvalid_(int *id)
{
	return(vwinidvalid(*id));
}
