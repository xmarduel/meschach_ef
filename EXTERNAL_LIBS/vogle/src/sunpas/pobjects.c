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
 * MakeObj
 */
void
MakeObj(int n)
{
	makeobj(n);
}

/*
 * AppendObj
 */
void
AppendObj(int n)
{
	appendobj(n);
}

/*
 * CloseObj
 */
void
CloseObj(void)
{
	closeobj();
}

/*
 * DelObj
 */
void
DelObj(int n)
{
	delobj(n);
}

/*
 * GenObj
 */
int
GenObj(void)
{
	return(genobj());
}

/*
 * GetOpenObj
 */
int
GetOpenObj(void)
{
	return(getopenobj());
}

/*
 * CallObj
 */
void
CallObj(int n)
{
	callobj(n);
}

/*
 * IsObj
 */
int
IsObj(int n)
{
	return(isobj(n));
}

/*
 * SaveObj
 */
void
SaveObj(int n, Vstring s)
{
	char	*p = (char *)s.s;
	char	buf[BUFSIZ];

        COPYANDTERMINATE(buf, p, s.n);

	saveobj(n, buf);
}

/*
 * LoadObj
 */
void
LoadObj(int n, Vstring s)
{
	char	*p = (char *)s.s;
	char	buf[BUFSIZ];

        COPYANDTERMINATE(buf, p, s.n);

	loadobj(n, buf);
}

