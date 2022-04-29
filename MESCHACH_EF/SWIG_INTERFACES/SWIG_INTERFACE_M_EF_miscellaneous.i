
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////  


#ifndef MEF_MISCELLANEOUS_1D_H
#define MEF_MISCELLANEOUS_1D_H

/* ------------------------------------------------------------------------ */

VEC* build_vec_from_function1D          (const ELT_1D *MyElt, const GEOM_1D *MyGeom, FUN_1D *fun, FUN_1D *dfun, VEC *vec);
VEC* build_vec_from_function1Dtransient (const ELT_1D *MyElt, const GEOM_1D *MyGeom, FUN_1D *fun, FUN_1D *dfun, VEC *vec, Real tps);
/* ------------------------------------------------------------------------ */

VEC* get_vector_for_plotting_1D(const ELT_1D *MyElt, const GEOM_1D *MyGeom, VEC *vec);

/* ------------------------------------------------------------------------ */

#endif


#ifndef MEF_MISCELLANEOUS_2D_H
#define MEF_MISCELLANEOUS_2D_H

/* ------------------------------------------------------------------------ */

VEC* build_vec_from_function2D          (const ELT_2D *MyElt, const GEOM_2D *MyGeom, FUN_2D* fun, VEC *vec);
VEC* build_vec_from_function2Dtransient (const ELT_2D *MyElt, const GEOM_2D *MyGeom, FUN_2D* fun, VEC *vec, Real tps);

/* ------------------------------------------------------------------------ */

#endif


#ifndef MEF_MISCELLANEOUS_3D_H
#define MEF_MISCELLANEOUS_3D_H

/* ------------------------------------------------------------------------ */

VEC* build_vec_from_function3D         (ELT_3D *elt, GEOM_3D *geom, FUN_3D* fun, VEC *vec);
VEC* build_vec_from_function3Dtransient(ELT_3D *elt, GEOM_3D *geom, FUN_3D* fun, VEC *vec, Real tps);

/* ------------------------------------------------------------------------ */

#endif