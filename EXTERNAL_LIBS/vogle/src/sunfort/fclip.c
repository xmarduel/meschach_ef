#include "vogle.h"

/*
 * clipping
 *
 *	turn clipping on/off
 */
void
clipping_(int *onoff)
{
	vdevice.clipoff = !*onoff;
}

/*
 * setpicksize_
 */
void
setpicksize_(float *sx, float *sy)
{
	setpicksize(*sx, *sy);
}

/*
 * setpickat_
 */
void
setpickat_(float *sx, float *sy)
{
	setpickat(*sx, *sy);
}

/*
 * pickmode_
 */
void
pickmode_(int *yes)
{
	pickmode(*yes);
}

/*
 * picked_
 */
int
picked_(void)
{
	return(picked());
}
