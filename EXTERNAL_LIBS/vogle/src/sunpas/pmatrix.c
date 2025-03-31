#include "vogle.h"

/*
 * PushMatrix
 */
void
PushMatrix(void)
{
	pushmatrix();
}

/*
 * PopMatrix
 */
void
PopMatrix(void)
{
	popmatrix();
}

/*
 * GetMatrix
 */
void
GetMatrix(Matrix mat)
{
	getmatrix(mat);
}

/*
 * LoadMatrix
 */
void
LoadMatrix(Matrix mat)
{
	loadmatrix(mat);
}

/*
 * MultMatrix
 */
void
MultMatrix(Matrix mat)
{
	multmatrix(mat);
}
