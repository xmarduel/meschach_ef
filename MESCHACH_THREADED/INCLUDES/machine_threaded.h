#ifndef MESCHACH_THREADED__H
#define MESCHACH_THREADED__H

/** \file meschach_threaded.h
*
* Contains the functions for threaded routines
*
* ---------------
* $Author: xavier $
* $Date: 2004/07/20 19:40:44 $
* $Revision: 1.5 $
*
* $Log: eigen.h,v $
* ------------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"


#ifdef __cplusplus
extern "C"
{
#endif


extern double in_prod_threaded(VEC *v1, VEC *v2);
extern VEC *  v_add_threaded(VEC *v1, VEC *v2, VEC *out);
extern VEC *  v_sub_threaded(VEC *v1, VEC *v2, VEC *out);
extern VEC *  sv_mlt_threaded(double s, VEC *v1, VEC *out);
extern VEC *  v_mltadd_threaded(VEC *v1,VEC *v2,double scale,VEC *out);
   
#ifdef __cplusplus
}
#endif

#endif
