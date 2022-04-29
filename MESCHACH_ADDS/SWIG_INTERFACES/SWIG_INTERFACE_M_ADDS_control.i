
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_ADDS
//
/////////////////////////////////////////////////////////

#ifndef CONTROL_H
#define CONTROL_H

#include "matrix.h"

#ifdef __cplusplus
extern "C" {
#endif
   

typedef enum
{
   CONTROLe_SLICOT = 1,
   CONTROLe_SCILAB = 2

} CONTROLt_METHOD ;

void set_control_method( int method );
int  get_control_method();

MAT *riccati  (const MAT *A, const MAT *B, const MAT *C, MAT *X);
MAT *lyapunov (const MAT *A, const MAT *C,               MAT *X);
MAT *sylvester(const MAT *A, const MAT *B, const MAT *C, MAT *X);

   
#ifdef __cplusplus
}
#endif

#endif