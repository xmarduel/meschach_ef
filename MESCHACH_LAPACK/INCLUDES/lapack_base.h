#ifndef MESCHACH_LAPACK__H
#define MESCHACH_LAPACK__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file lapack_base.h
 *
 * Contains the basic routines to convert meschach stucture to lapack arrays (fortran)
 *
 * Actually :\n
 *  conversion of a general   sparse meschach matrix ( SPMAT * ) to a lapack general   banded matrix\n
 *  conversion of a symmetric sparse meschach matrix ( SPMAT * ) to a lapack symmetric banded matrix\n
 *
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

MAT* m_transp2(MAT *eig_vectors); /* a routine to transpose "in-situ" non-square matrices */


void b_foutput(FILE* fp, const BAND* B);

BAND * sp2band     (const SPMAT *A ); /* conversion meschach SPMAT *A to meschach BAND B */
BAND * sp2lowerband(const SPMAT *A ); /* conversion meschach SPMAT *A to meschach BAND B ( and A is symmetric -> only the lower part ) */

MAT  * band2lapackbanded(const BAND* B ); /* conversion meschach BAND *B to meschach MAT *M */

MAT  * spmat2lapackbanded     (const SPMAT *A );
MAT  * spmat2lapacklowerbanded(const SPMAT *A );

/* for tests */
VEC * Ax_lapackbanded     (const SPMAT *A, const VEC * v1, VEC *v2);
VEC * Ax_lapacklowerbanded(const SPMAT *A, const VEC * v1, VEC *v2);


#ifdef __cplusplus
}
#endif


#endif

/*! \fn spmat2lapackbanded          (const SPMAT *A)
* 
* \param A : the sparse matrix to convert
*
* convert a "general"  sparse meschach matrix ( SPMAT * ) to a lapack "general"  banded matrix
*/

/*! \fn spmat2lapacklowerbanded          (const SPMAT *A)
*
* \param A : the sparse matrix to convert
*
* convert a "symmetric"  sparse meschach matrix ( SPMAT * ) to a lapack "symmetric" banded matrix ( only the lower part of the matrix )
*/

