#ifndef MEF_GRAPHICS_OUTPUT_2D__H
#define MEF_GRAPHICS_OUTPUT_2D__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics_output_2D.h
 *
 * Contains all the output files functions\n
 *
 * ----------------------------------------------------------------------- */

#include "MESCHACH_EF/INCLUDES/finite_elements_2D.h"
#include "MESCHACH_EF/INCLUDES/geometry_2D.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"


/*
 *   2-Dim
 */

void graphics2D_set   (const char *format, const ELT_2D *elt, const GEOM_2D *geom, const int nb, const MAT *SOL, const char *name_file);

void graphics2D       (const char* format, const ELT_2D *elt, const GEOM_2D *geom, const VEC    *SOL, const char *name_file);
void graphics2D_fun   (const char* format, const ELT_2D *elt, const GEOM_2D *geom, const FUN_2D *FUN, const char *name_file);

/* for stokes */
void graphics2D_stokes    (const char* format, const ELT_2D *elt, const GEOM_2D *geom, const VEC     *u, const VEC     *v, const VEC     *p , const char *name_file);
void graphics2D_stokes_fun(const char* format, const ELT_2D *elt, const GEOM_2D *geom, const FUN_2D *Fu, const FUN_2D *Fv, const FUN_2D *Fp , const char *name_file);

#ifdef __cplusplus
}
#endif

#endif

/*! \fn graphics2D (const char* format, const ELT_2D *element , const GEOM_2D *Geom , const VEC    *SOL , const char *name_file)
* 
* \param format    : the output-format ( "gnuplot", "matlab", "silo", "vtk" )
* \param element   : 
* \param Geom      : the geometry
* \param SOL       : the vector to plot
* \param name_file : the name of the file which is store the solution
*
* Two files created : the file containing the data, and a script-file.\n
* Examples:\n
* format is "vtk" and file_name is "SolApproch", the files creared are\n
*   1- Script4vtk_SolApproch2D.tcl \n
*   2- SolApproch2D.vtk \n
* \n
* format is "silo" and file_name is "SolApproch", the files creared are\n
*   1- Script4mtv_SolApproch2D.sil \n
*   2- SolApproch2D.silo \n
* \n
* format is "gnuplot" and file_name is "SolApproch", the files creared are\n
*   1- Script4gpl_SolApproch2D.gnu \n
*   2- SolApproch2D.dat \n
* \n
* format is "matlab" and file_name is "SolApproch", the files creared are\n
*   1- Script4matlab_SolApproch2D.m \n
*   2- SolApproch2D.m \n
* \n*/
