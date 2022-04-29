#ifndef SPOOLES_WRAPPER__H
#define SPOOLES_WRAPPER__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file spooles_wrapper.h
 * Contains the basic routines to convert meschach structures to spooles structures
 *
 *
 */
   
#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/iter.h"

#include "spooles/DV.h"
#include "spooles/DenseMtx.h"
#include "spooles/InpMtx.h"



DV       *v_meschach_to_v_spooles(const VEC *v, DV *dv);
DenseMtx *v_meschach_to_m_spooles(const VEC *v, DenseMtx* M);
DenseMtx *m_meschach_to_m_spooles(const MAT *A, DenseMtx* M);
InpMtx   *s_meschach_to_s_spooles(const SPMAT *A, InpMtx *B);
InpMtx   *s_meschach_to_s_spooles_sym(const SPMAT *A, InpMtx *B);

VEC      *v_spooles_to_v_meschach(const DV      *dv, VEC   *v);
VEC      *m_spooles_to_v_meschach(const DenseMtx *M, VEC   *v);
MAT      *m_spooles_to_m_meschach(const DenseMtx *A, MAT   *M);
SPMAT    *s_spooles_to_s_meschach(const InpMtx   *A, SPMAT *S);


#ifdef __cplusplus
}
#endif

#endif

/*! \fn v_meschach_to_v_spooles          (const VEC *v, DV *dv)
* \brief
* \param v    : input : the meschach vector
* \param dv   : output : the spooles vector ( can be NULL in input if created )
*
* convert a "meschach" vector to a "spooles" vector
*/

/*! \fn v_meschach_to_m_spooles          (const VEC *v, DenseMtx* M)
* \brief
* \param v    : input : the meschach vector
* \param M    : output : the spooles matrix ( can be NULL in input if created )
*
* convert a "meschach" vector to a "spooles" matrix
*/

/*! \fn m_meschach_to_m_spooles          (const MAT *A, DenseMtx* M)
* \brief
* \param A    : input : the meschach matrix
* \param M    : output : the spooles matrix ( can be NULL in input if created )
*
* convert a "meschach" matrix to a "spooles" matrix
*/

/*! \fn s_meschach_to_s_spooles          (const SPMAT *A, InpMtx *B)
* \brief
* \param A    : input : the meschach matrix
* \param B    : output : the spooles sparse matrix ( can be NULL in input if created )
*
* convert a "meschach" sparse matrix to a "spooles" sparse matrix
*/

/*! \fn s_meschach_to_s_spooles_sym          (const SPMAT *A, InpMtx *B)
* \brief
* \param A    : input : the meschach matrix
* \param B    : output : the spooles sparse matrix - only the lower part - ( can be NULL in input if created )
*
* convert a "meschach" sparse matrix to a "spooles" sparse matrix. Consider only the lower part of A
*/



/*! \fn v_spooles_to_v_meschach          (const DV       *dv, VEC   *v)
* \brief
* \param dv   : input  : the spooles  vector
* \param v    : output : the meschach vector ( can be NULL in input if created )
*
* convert a "spooles" vector to a "meschach" vector
*/

/*! \fn m_spooles_to_v_meschach          (const DenseMtx *M, VEC   *v)
* \brief
* \param M    : input : the spooles matrix
* \param v    : output : the meschach vector ( can be NULL in input if created )
*
* convert a "spooles" matrix to a "meschach" vector
*/

/*! \fn m_spooles_to_m_meschach          (const DenseMtx *A, MAT   *M)
* \brief
* \param A    : input : the spooles matrix
* \param M    : input : the meschach matrix ( can be NULL in input if created )
*
* convert a "spooles" matrix to a "meschach" matrix
*/

/*! \fn s_spooles_to_s_meschach         (const InpMtx   *A, SPMAT *S)
* \brief
* \param A    : input : the spooles matrix
* \param S    : input : the meschach matrix ( can be NULL in input if created )
*
* convert a "spooles" sparse matrix to a "meschach" sparse matrix
*/

