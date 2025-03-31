#include "vogle.h"
/*
 * BackBuffer
 */
int
BackBuffer(void)
{
	return(backbuffer());
}

/*
 * FrontBuffer
 */
void
FrontBuffer(void)
{
	frontbuffer();
}

/*
 * SwapBuffers
 */
int
SwapBuffers(void)
{
	return(swapbuffers());
}
