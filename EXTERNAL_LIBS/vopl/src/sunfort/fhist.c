#include "vopl.h"

/*
 * hist2_
 */
hist2_(d, ng, ns, names)
	float	*d;
	int	ng;
	int	ns;
	char	*names[];
{
	hist2(d, ng, ns, names);
}

/*
 * histskip_
 */
histskip_(h)
	int	*h;
{
	histskip(h);
}
