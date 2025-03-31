#include "vopl.h"

/*
 * newm1
 *
 *	allocate a one dimensional array
 */
float *
newm1(
	int	n)
{
	float	*p;

	if ((p = (float *)malloc(sizeof(float) * n)) == (float *)NULL) {
		fprintf(stderr, "newm1: request for %d bytes returns NULL\n", n);
		exit(1);
	}

	return(p);
}

/*
 * newm2
 *
 *	allocate an array of arrays
 */
float **
newm2(
	int	m,
	int	n)
{
	float  **mat;
	int i;

	if ((mat = (float **)malloc((unsigned)sizeof(float *) * m)) == (float **)NULL) {
		fprintf(stderr, "newm2: request for %d bytes returns NULL\n", n);
		exit(1);
	}

	for (i = 0; i < m; i++)
		if ((mat[i] = (float *)malloc(n * sizeof(float))) == (float *)NULL) {
			fprintf(stderr, "newm2: request for %d bytes returns NULL\n", n);
			exit(1);
		}

	return(mat);
}
