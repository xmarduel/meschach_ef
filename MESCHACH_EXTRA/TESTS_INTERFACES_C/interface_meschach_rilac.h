#ifndef INTERFACE_MESCHACH_RILAC_H
#define INTERFACE_MESCHACH_RILAC_H

/* Equation KS  1D
 *
 * ---------------
 * $Author: Xavier Marduel $
 * $Date: 23/02/2000 $
 * $Revision: 1.0 $
 * ------------------------------------------------------------------------ */

MAT *interface_meschach_rilac(MAT *A, MAT *B, MAT *C, MAT *X); /* riccati   */
MAT *interface_meschach_lybsc(MAT *A, MAT *C, MAT *X);         /* lyapunov  */
MAT *interface_meschach_syhsc(MAT *A, MAT *B, MAT *C, MAT *X); /* sylvester */

#endif
