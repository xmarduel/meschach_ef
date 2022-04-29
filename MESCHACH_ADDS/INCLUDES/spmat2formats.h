#ifndef SPMAT_TO_FORMATS_H
#define SPMAT_TO_FORMATS_H


#ifdef __cplusplus
extern "C" {
#endif

/** \file spmat2formats.h
 *
 * Contains routines to transform Meschach "SPMAT" to "FORTRAN" arrays for sparse matrices
 *
 * ------------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"
   

void  spmat2crs( const SPMAT *A ,       VEC *a ,       IVEC *col_ind ,       IVEC *row_ptr , int type /* 'C' or 'F' */);
void  crs2spmat(       SPMAT *A , const VEC *a , const IVEC *col_ind , const IVEC *row_ptr , int type /* 'C' or 'F' */);

void  spmat2ccs( const SPMAT *A ,       VEC *a ,       IVEC *row_ind ,       IVEC *col_ptr , int type /* 'C' or 'F' */);
void  ccs2spmat(       SPMAT *A , const VEC *a , const IVEC *row_ind , const IVEC *col_ptr , int type /* 'C' or 'F' */);

void  spmat2mrs( const SPMAT *A ,       VEC *a,       IVEC *col_ind );
void  mrs2spmat(       SPMAT *A , const VEC *a, const IVEC *col_ind );



#ifdef __cplusplus
}
#endif

#endif
