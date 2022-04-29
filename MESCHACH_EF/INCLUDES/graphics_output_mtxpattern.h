#ifndef MEF_GRAPHICS_OUTPUT_MTXPATTERN__H
#define MEF_GRAPHICS_OUTPUT_MTXPATTERN__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics_output_mtxpattern.h
 *
 * Contains functions for plotting matrix pattern (with gnuplot or graph apps)
 *
 * ----------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/sparse.h"


/*
 * matrix patterns (with gnuplot or graph)
 */

void graphics_matrix_pattern( const char* format, const SPMAT* A, const char* name_file);

#ifdef __cplusplus
}
#endif

#endif

/*! \fn graphics_matrix_pattern(const char* format, const SPMAT* A, const char* name_file);
* 
* \param format    : "X", "gnuplot", "graph"
* \param A         : the sparse matrix
* \param name_file : if not "X", the file to be read by "gnuplot" or "graph" script
*
*/

