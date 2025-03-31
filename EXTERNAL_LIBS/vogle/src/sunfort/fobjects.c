#include <stdio.h>
#include <string.h>
#include "vogle.h"

/*
 * makeobj_
 */
void
makeobj_(int *n)
{
	makeobj(*n);
}

/*
 * appendobj_
 */
void
appendobj_(int *n)
{
	appendobj(*n);
}

/*
 * closeobj_
 */
void
closeobj_(void)
{
	closeobj();
}

/*
 * delobj_
 */
void
delobj_(int *n)
{
	delobj(*n);
}

/*
 * genobj_
 */
int
genobj_(void)
{
	return(genobj());
}

/*
 * getopenobj_
 */
int
getopenobj_(void)
{
	return(getopenobj());
}

/*
 * callobj_
 */
void
callobj_(int *n)
{
	callobj(*n);
}

/*
 * isobj_
 */
int
isobj_(int *n)
{
	return(isobj(*n));
}

/*
 * saveobj_
 */
void
saveobj_(int *n, char *file, int len)
{
	char		buf[BUFSIZ];
	register char	*p;

	strncpy(buf, file, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	saveobj(*n, buf);
}

/*
 * loadobj_
 */
void
loadobj_(int *n, char *file, int len)
{
	char		buf[BUFSIZ];
	register char	*p;

	strncpy(buf, file, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	loadobj(*n, buf);
}

