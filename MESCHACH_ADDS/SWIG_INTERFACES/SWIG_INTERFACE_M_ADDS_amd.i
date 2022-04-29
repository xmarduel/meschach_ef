#ifndef AMD_WRAPPER_H__
#define AMD_WRAPPER_H__


#include "matrix.h"
#include "sparse.h"

#ifdef __cplusplus
extern "C" {
#endif



PERM* sp_bandwr_amd(SPMAT *A, PERM* P, PERM *INVP, int type); 



#ifdef __cplusplus
}
#endif


#endif