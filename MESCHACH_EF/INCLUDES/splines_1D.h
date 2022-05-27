#ifndef SPLINES_1D_H
#define SPLINES_1D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file splines_1D.h
 *
 * Contains the structure of SPLINES in 1 dimension for non-uniform meshes\n
 *
 *
 * The definition of a "whole" spline on a "whole" mesh is given with the data
 * of a set of "basic" splines defined only on a "small" mesh. For example,\n
 * -> Spline "S2" : a basic spline is defined with the data of 3 intervals (4 pts)\n
 * -> Spline "S3" : a basic spline is defined with the data of 4 intervals (5 pts)\n
 * -> Spline "S5" : a basic spline is defined with the data of 6 intervals (7 pts)\n
 *
 * The "basic" mesh ( 3 intervals for "S2", 4 intervals for "S3" or 6 intervals for "S5" ) is also called "local mesh" \n
 *
 * The definition of a basic spline on this local mesh is given with a matrix
 * ( 3x3 for "S2", 4x4 for "S3" or 6x6 for "S5" ). For example :\n
 * -> the first line stores the 4 coeff. of { 1, X, X**2, X**3 } on the first interval\n
 * -> the second line     " "           " "                      on the second interval\n
 * -> the third line      " "           " "                      on the third interval\n
 * -> the fourth line     " "           " "                      on the fourth interval\n
 *
 * There are 2 possibilities for the basic spline :\n
 * \n
 * 1 - The spline has zero values at the left & right sides on the local mesh, and also zero derivatives left and right.\n  
 * The spline also goes throught the FIXED points (X0,0), (X1,1), (X2,4), (X3,1), (X4,0)\n
 * The spline is C-1 on the local mesh ( and on the whole mesh) (instead of being C-2 in the case of an uniform mesh)\n
 * The basic spline is not C-2 at X0 and X4 but C-2 eerywhere else ( the "whole" spline will be only C-1)\n
 * \n
 * 2- The spline has zero values at the left & right sides on the local mesh, and also zero derivatives left and right,\n
 * AND ALSO ZERO SECOND DERIVATES left and right\n
 * The spline also goes throught the FIXED points (X0,0), (X2,4), (X4,0). The coordinates Y1 and Y3 are FREE.\n
 * The spline is consequently C-2, as with a uniform mesh, bur Y1 and Y3 are free.\n
 * \n
 * Which one to choose ONE or TWO  ????\n
 *
 *
 * A third possibility ( not aceptable for our purpose ) is\n
 *
 * 3- The spline has zero values at the left & right sides on the local mesh, but FREE first derivatives left and right,\n
 * AND ALSO ZERO SECOND DERIVATES left and right.\n
 * The spline also goes throught the FIXED points (X0,0), (X1,1), (X2,4), (X3,1), (X4,0).\n
 * This is a "natural" spline. with prolongation other the "whole" mesh, the spline is only C-0 ( only continious at X0 and X4)\n
 * ( and its second derivative is continious too! )\n
 * \n
 * \n
 * See files\n
 * CALC_SPLINES_GENERAL.py (case 1), \n
 * CALC_SPLINES_NEW.py     (case 2) \n
 * CALC_SPLINES_NATURAL.py (case 3)\n
 * and gnuplot outputs\n
 * ------------------------------------------------------------------------ */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_1D.h"
#include "MESCHACH_EF/INCLUDES/geometry_1D.h"

/*------------------------------------------------------------------------- */


typedef struct {

   MAT* COEFFS_POLYNOMES;
   VEC* LOCAL_MESH;

} SPLINE_1D_BASIC ;


typedef struct {

   char type[4];

   SPLINE_1D_BASIC *spline_basic;
   int dim;

} SPLINE_1D ;


#define SPLINE_1D_NULL         ((SPLINE_1D *)NULL)
#define SPLINE_1D_FREE(spl)    ( spline1D_free(spl),  (spl)=SPLINE_1D_NULL )

SPLINE_1D *spline1D_get  (const ELT_1D *elt, const GEOM_1D *geom);

int        spline1D_free (SPLINE_1D  *spl);

 


#define SPLINE_1D_BASIC_NULL         ((SPLINE_1D_BASIC *)NULL)
#define SPLINE_1D_BASIC_FREE(spl)    ( spline1D_basic_free(spl),  (spl)=SPLINE_1D_BASIC_NULL )


SPLINE_1D_BASIC *spline1D_basic_get  (const ELT_1D *elt);
SPLINE_1D_BASIC *spline1D_basic_init (SPLINE_1D_BASIC *spl, const char *name_ef);
SPLINE_1D_BASIC *spline1D_basic_calc (SPLINE_1D_BASIC *spl, const char *name_ef, const VEC *mesh);

int              spline1D_basic_free (SPLINE_1D_BASIC  *spl);

#ifdef __cplusplus
}
#endif

#endif




