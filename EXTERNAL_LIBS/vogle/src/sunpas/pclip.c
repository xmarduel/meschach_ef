#include "vogle.h"

/*
 * Clipping
 *
 *	turn clipping on/off
 */
void
Clipping(int onoff)
{
	vdevice.clipoff = !onoff;
}

/*
 * SetPicksize
 */
void
SetPicksize(float sx, float sy)
{
        setpicksize(sx, sy);
}

/*
 * SetPickat
 */
void
SetPickat(float sx, float sy)
{
        setpickat(sx, sy);
}

/*
 * PickMode
 */
void
PickMode(int yes)
{
        pickmode(yes);
}

/*
 * Picked
 */
int
Picked(void)
{
        return(picked());
}

