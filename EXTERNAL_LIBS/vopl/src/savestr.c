#include "vogle.h"
#include "vopl.h"
#include <string.h>

/*
 * savestr
 *
 *	Save a string of characters somewhere
 */
char *
savestr(
	char	*old,
	char	*string)
{
	char	*p;

	if (string == (char *)NULL)
		return ((char *)NULL);

	p = old;

	if (p != (char *)NULL)
		free(p);

	p = strdup(string);

	return(p);
}
