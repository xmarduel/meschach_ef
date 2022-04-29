
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_ADDS
//
/////////////////////////////////////////////////////////
 
#ifndef SPMAT_TO_FORMATS_H
#define SPMAT_TO_FORMATS_H

#include "matrix.h"
#include "sparse.h"

#ifdef __cplusplus
extern "C" {
#endif


void  spmat2crs( SPMAT *A , VEC *a , IVEC *col_ind , IVEC *row_ptr , int type /* 'C' or 'F' */);
void  crs2spmat( SPMAT *A , VEC *a , IVEC *col_ind , IVEC *row_ptr , int type /* 'C' or 'F' */);

void  spmat2ccs( SPMAT *A , VEC *a , IVEC *col_ind , IVEC *row_ptr , int type /* 'C' or 'F' */);
void  ccs2spmat( SPMAT *A , VEC *a , IVEC *col_ind , IVEC *row_ptr , int type /* 'C' or 'F' */);

void  spmat2mrs( SPMAT *A , VEC *a, IVEC *col_ind );
void  mrs2spmat( SPMAT *A , VEC *a, IVEC *col_ind );


#ifdef __cplusplus
}
#endif

#endif