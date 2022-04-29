
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_ADDS
//
///////////////////////////////////////////////////////// 
 
#ifndef GPSKMESH_H
#define GPSKMESH_H

#include "matrix.h"
#include "sparse.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {

   BANDWRe_MESCHACH_BAND    =  0,
   BANDWRe_MESCHACH_PROFILE =  1

} BANDWRt_OPTION_MESCHACH ;

PERM* sp_bandwr_meschach(const SPMAT *A, PERM* P, PERM *INVP, BANDWRt_OPTION_MESCHACH option);

#ifdef __cplusplus
}
#endif

#endif