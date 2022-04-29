#ifndef GPSKMESH_H
#define GPSKMESH_H


#ifdef __cplusplus
extern "C" {
#endif


#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

/** \file gpskmesh.h
 *
 * Contains the function to call "gpskmesh" main function (band width reduction)
 *
 * ------------------------------------------------------------------------ */

typedef enum {

   BANDWRe_MESCHACH_BAND    =  0,
   BANDWRe_MESCHACH_PROFILE =  1

} BANDWRt_OPTION_MESCHACH ;

PERM* sp_bandwr_meschach(const SPMAT *A, PERM* P, PERM *INVP, BANDWRt_OPTION_MESCHACH option);

#ifdef __cplusplus
}
#endif

#endif


/*! \enum BANDWRt_OPTION_MESCHACH
* \brief List the different types of option for the band width reduction
*/

/*! \fn PERM * sp_bandwr_meschach(const SPMAT *A, PERM* P, PERM *INVP, BANDWRt_OPTION_MESCHACH option);
* \brief Evaluate the permutation to perform a band/profile reduction
*
* \param A :
* \param P : the calculated permutation
* \param INVP : and its inverse
* \param option : option for band width reduction
*
* calculates the permutation to reduce the band width/profile of A when doing B = P.A.Pt 
*/
