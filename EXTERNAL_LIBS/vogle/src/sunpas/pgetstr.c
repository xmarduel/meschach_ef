#include <stdio.h>
#include "vogle.h"

/*
 * This should match the Pascal type 'varying [n] of char'
 */

typedef struct {
	int	n;
	char	s[1];	/* SO compiler doesn't bitch about zero length */
} Vstring;

/*
 * GetString
 */
int
GetString(int bcol, Vstring *s)
{
	char	*p = (char *)s->s;

	s->n = getstring(bcol, p);

	return (s->n);
}
