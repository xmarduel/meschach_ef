#ifndef AMD_WRAPPER_H__
#define AMD_WRAPPER_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

/** \file amd_wrapper.h
 *
 * Contains the function to call "amd" main function (band width reduction)
 *
 * ------------------------------------------------------------------------ */

PERM* sp_bandwr_amd(const SPMAT *A, PERM* P, PERM *INVP, int type);


#ifdef __cplusplus
}
#endif


#endif

/*! \fn PERM * sp_bandwr_amd(const SPMAT *A, PERM* P, PERM *INVP, int type);
* \brief Evaluate the permutation to perform a band/profile reductioni
* 
* \param A :
* \param P : the calculated permutation
* \param INVP : and its inverse
* \param type : option for band width reduction - unused actually -
*
* calculates the permutation to reduce the band width of A when doing B = P.A.Pt
*/

