#ifndef SPOOLES_WRAPPER_FACTOR__H
#define SPOOLES_WRAPPER_FACTOR__H

#ifdef __cplusplus
extern "C"
{
#endif


/** \file spooles_wrapper_factor.h
 *
 * Contains the routines for solving A.x = b with SPOOLES (with band width reduction)
 * 
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"


typedef enum {

   BANDWRe_SPOOLES_SYM     =  0,
   BANDWRe_SPOOLES_NONSYM  =  1

} BANDWRt_SPOOLES_OPTION ;


VEC* spCHresolution_spooles( const SPMAT *A, const VEC *in, VEC *out);
VEC* spLUresolution_spooles( const SPMAT *A, const VEC *in, VEC *out);

PERM* sp_bandwr_spooles(const SPMAT *A, PERM *P, PERM *INVP, BANDWRt_SPOOLES_OPTION type);

#ifdef __cplusplus
}
#endif

#endif

/*! \enum BANDWRt_SPOOLES_OPTION
* \brief
*
* only used to distinguish the cases where the sparse matrix is symetric or not (only an half is then really stored)
*/

/*! \fn sp_bandwr_spooles (const SPMAT *A, PERM *P, PERM *INVP, BANDWRt_SPOOLES_OPTION type);
* \brief
* \param A    : input  : the sparse matrix
* \param P    : ouput  : the permutation ...
* \param INVP : output : and its inverse
* \param type : NOT USED
*
* Calculate the permutation minimizing the profile of a sparse matrix A (supposed non-symmetric)
*/

/*! \fn spCHresolution_spooles( const SPMAT *A, const VEC *in, VEC *out);
* \brief
* \param A   : input  : the sparse matrix
* \param in  : input  : the vector "rhs"
* \param out : output : the solution
*
* Solve A.x = b with spooles ( A symmetric )
*/

/*! \fn spLUresolution_spooles ( const SPMAT *A, const VEC *in, VEC *out);
* \brief
* \param A   : input  : the sparse matrix
* \param in  : input  : the vector "rhs"
* \param out : output : the solution
*
* Solve A.x = b with spooles ( A non-symmetric )
*/

