
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////  

#ifndef MEF_PROBLEMS_SOLVER_H
#define MEF_PROBLEMS_SOLVER_H

#include "matrix.h"

#include "finite_elements.h"
#include "geometry.h"
#include "boundary_conditions.h"
#include "rhs.h"
#include "adv.h"

VEC*  solve1D_laplacian           (ELT_1D *elt , GEOM_1D *MyGeom, BC_1D *MyBC, RHS_1D *MyRhsFun );
VEC*  solve1D_bilaplacian         (ELT_1D *elt , GEOM_1D *MyGeom, BC_1D *MyBC, RHS_1D *MyRhsFun );
VEC*  solve1D_convection_diffusion(ELT_1D *elt , GEOM_1D *MyGeom, BC_1D *MyBC, RHS_1D *MyRhsFun , ADV_1D *MyAdvFun );
VEC*  solve1D_helmholz            (ELT_1D *elt , GEOM_1D *MyGeom, BC_1D *MyBC, RHS_1D *MyRhsFun );
VEC*  solve1D_burgers             (ELT_1D *elt , GEOM_1D *MyGeom, BC_1D *MyBC, RHS_1D *MyRhsFun );


VEC*  solve2D_laplacian           (ELT_2D *elt , GEOM_2D *MyGeom, BC_2D *MyBC, RHS_2D *MyRhsFun );
VEC*  solve2D_helmholz            (ELT_2D *elt , GEOM_2D *MyGeom, BC_2D *MyBC, RHS_2D *MyRhsFun );
VEC*  solve2D_convection_diffusion(ELT_2D *elt , GEOM_2D *MyGeom, BC_2D *MyBC, RHS_2D *MyRhsFun , ADV_2D *MyAdvFun );

VEC*  solve3D_laplacian           (ELT_3D *elt , GEOM_3D *MyGeom, BC_3D *MyBC, RHS_3D *MyRhsFun );
VEC*  solve3D_helmholz            (ELT_3D *elt , GEOM_3D *MyGeom, BC_3D *MyBC, RHS_3D *MyRhsFun );
VEC*  solve3D_convection_diffusion(ELT_3D *elt , GEOM_3D *MyGeom, BC_3D *MyBC, RHS_3D *MyRhsFun , ADV_3D *MyAdvFun );


void  solve2D_stokes        (ELT_2D *elt2 , ELT_2D *elt1 , GEOM_2D *MyGeom, BC_2D *MyBC, RHS_2D *MyRhsFun );
void  solve2D_stokes_       (ELT_2D *elt2 , ELT_2D *elt1 , GEOM_2D *MyGeom, BC_2D *MyBC, RHS_2D *MyRhsFun, VEC *U, VEC *V, VEC *P );
void  solve2D_navier_stokes (ELT_2D *elt2 , ELT_2D *elt1 , GEOM_2D *MyGeom, BC_2D *MyBC, RHS_2D *MyRhsFun );
void  solve2D_navier_stokes_(ELT_2D *elt2 , ELT_2D *elt1 , GEOM_2D *MyGeom, BC_2D *MyBC, RHS_2D *MyRhsFun, VEC *U, VEC *V, VEC *P );

void  solve3D_stokes        (ELT_3D *elt2 , ELT_3D *elt1 , GEOM_3D *MyGeom, BC_3D *MyBC, RHS_3D *MyRhsFun );
void  solve3D_stokes_       (ELT_3D *elt2 , ELT_3D *elt1 , GEOM_3D *MyGeom, BC_3D *MyBC, RHS_3D *MyRhsFun, VEC *U, VEC *V, VEC* W, VEC *P );
void  solve3D_navier_stokes (ELT_3D *elt2 , ELT_3D *elt1 , GEOM_3D *MyGeom, BC_3D *MyBC, RHS_3D *MyRhsFun );
void  solve3D_navier_stokes_(ELT_3D *elt2 , ELT_3D *elt1 , GEOM_3D *MyGeom, BC_3D *MyBC, RHS_3D *MyRhsFun, VEC *U, VEC *V, VEC* W, VEC *P );


//////////////////////////////////////////////////////////////////////
////  MACROS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

%inline %{


PyObject *Py_solve2D_stokes(ELT_2D *eltP2, ELT_2D *eltP1, GEOM_2D *Geom, BC_2D *Bc, RHS_2D *Rhs)
{
	PyObject *resultobj;

	VEC* U;
	VEC* V;
	VEC* P;
		
	int exception_status = 0;

	if ((exception_status=setjmp(*jmp_buf__get())) == 0)
	{
		if ( eltP2          == NULL ) error(E_NULL, "Py_solve2D_stokes");
		if ( eltP1          == NULL ) error(E_NULL, "Py_solve2D_stokes");
		if ( Geom           == NULL ) error(E_NULL, "Py_solve2D_stokes");
		if ( Bc             == NULL ) error(E_NULL, "Py_solve2D_stokes");
		if ( Rhs            == NULL ) error(E_NULL, "Py_solve2D_stokes");

		if ( Geom->geomBase == NULL ) error3(E_GEOM_NOCHILDGEOM, "Py_solve2D_stokes");
			
		U = v_get(Geom->NBSOMM);
		V = v_get(Geom->NBSOMM);
		P = v_get(Geom->geomBase->NBSOMM);

		(void) solve2D_stokes_(eltP2, eltP1, Geom, Bc, Rhs, U, V, P);
	}
	else
	{
           return exception_catcher(exception_status);
        }
    
    
	resultobj = Py_BuildValue("(OOO)",SWIG_NewPointerObj((void *) U, SWIGTYPE_p_VEC, 0),
	                                  SWIG_NewPointerObj((void *) V, SWIGTYPE_p_VEC, 0),
	                                  SWIG_NewPointerObj((void *) P, SWIGTYPE_p_VEC, 0));
        return resultobj;
}


PyObject *Py_solve2D_navier_stokes(ELT_2D *eltP2, ELT_2D *eltP1, GEOM_2D *Geom, BC_2D *Bc, RHS_2D *Rhs)
{
	PyObject *resultobj;

	VEC* U;
	VEC* V;
	VEC* P;
		
	int exception_status = 0;

	if ((exception_status=setjmp(*jmp_buf__get())) == 0)
	{
		if ( eltP2          == NULL ) error(E_NULL, "Py_solve2D_navierstokes");
		if ( eltP1          == NULL ) error(E_NULL, "Py_solve2D_navierstokes");
		if ( Geom           == NULL ) error(E_NULL, "Py_solve2D_navierstokes");
		if ( Bc             == NULL ) error(E_NULL, "Py_solve2D_navierstokes");
		if ( Rhs            == NULL ) error(E_NULL, "Py_solve2D_navierstokes");

		if ( Geom->geomBase == NULL ) error3(E_GEOM_NOCHILDGEOM, "Py_solve2D_navierstokes");
			
		U = v_get(Geom->NBSOMM);
		V = v_get(Geom->NBSOMM);
		P = v_get(Geom->geomBase->NBSOMM);

		(void) solve2D_navier_stokes_(eltP2, eltP1, Geom, Bc, Rhs, U, V, P);
	}
	else
	{
                return exception_catcher(exception_status);
        }
    
	resultobj = Py_BuildValue("(OOO)",SWIG_NewPointerObj((void *) U, SWIGTYPE_p_VEC, 0),
	                                  SWIG_NewPointerObj((void *) V, SWIGTYPE_p_VEC, 0),
	                                  SWIG_NewPointerObj((void *) P, SWIGTYPE_p_VEC, 0));
        return resultobj;
}


PyObject *Py_solve3D_stokes(ELT_3D *eltP2, ELT_3D *eltP1, GEOM_3D *Geom, BC_3D *Bc, RHS_3D *Rhs)
{
	PyObject *resultobj;

	VEC* U;
	VEC* V;
	VEC* W;
	VEC* P;
		
	int exception_status = 0;

	if ((exception_status=setjmp(*jmp_buf__get())) == 0)
	{
		if ( eltP2          == NULL ) error(E_NULL, "Py_solve3D_stokes");
		if ( eltP1          == NULL ) error(E_NULL, "Py_solve3D_stokes");
		if ( Geom           == NULL ) error(E_NULL, "Py_solve3D_stokes");
		if ( Bc             == NULL ) error(E_NULL, "Py_solve3D_stokes");
		if ( Rhs            == NULL ) error(E_NULL, "Py_solve3D_stokes");
		
		if ( Geom->geomBase == NULL ) error3(E_GEOM_NOCHILDGEOM, "Py_solve3D_stokes");

		U = v_get(Geom->NBSOMM);
		V = v_get(Geom->NBSOMM);
		W = v_get(Geom->NBSOMM);
		P = v_get(Geom->geomBase->NBSOMM);

		(void) solve3D_stokes_(eltP2, eltP1, Geom, Bc, Rhs, U, V, W, P);
	}
	else
	{
                return exception_catcher(exception_status);
        }
    
	resultobj = Py_BuildValue("(OOOO)",SWIG_NewPointerObj((void *) U, SWIGTYPE_p_VEC, 0),
	                                   SWIG_NewPointerObj((void *) V, SWIGTYPE_p_VEC, 0),
	                                   SWIG_NewPointerObj((void *) W, SWIGTYPE_p_VEC, 0),
                                           SWIG_NewPointerObj((void *) P, SWIGTYPE_p_VEC, 0));
	return resultobj;
}

PyObject *Py_solve3D_navier_stokes(ELT_3D *eltP2, ELT_3D *eltP1, GEOM_3D *Geom, BC_3D *Bc, RHS_3D *Rhs)
{
	PyObject *resultobj;

	VEC* U;
	VEC* V;
	VEC* W;
	VEC* P;
		
	int exception_status = 0;

	if ((exception_status=setjmp(*jmp_buf__get())) == 0)
	{
		if ( eltP2          == NULL ) error(E_NULL, "Py_solve3D_navierstokes");
		if ( eltP1          == NULL ) error(E_NULL, "Py_solve3D_navierstokes");
		if ( Geom           == NULL ) error(E_NULL, "Py_solve3D_navierstokes");
		if ( Bc             == NULL ) error(E_NULL, "Py_solve3D_navierstokes");
		if ( Rhs            == NULL ) error(E_NULL, "Py_solve3D_navierstokes");
		
		if ( Geom->geomBase == NULL ) error3(E_GEOM_NOCHILDGEOM, "Py_solve3D_navierstokes");

		U = v_get(Geom->NBSOMM);
		V = v_get(Geom->NBSOMM);
		W = v_get(Geom->NBSOMM);
		P = v_get(Geom->geomBase->NBSOMM);

		(void) solve3D_navier_stokes_(eltP2, eltP1, Geom, Bc, Rhs, U, V, W, P);
	}
	else
	{
                return exception_catcher(exception_status);
        }
    
	resultobj = Py_BuildValue("(OOOO)",SWIG_NewPointerObj((void *) U, SWIGTYPE_p_VEC, 0),
	                                   SWIG_NewPointerObj((void *) V, SWIGTYPE_p_VEC, 0),
	                                   SWIG_NewPointerObj((void *) W, SWIGTYPE_p_VEC, 0),
                                           SWIG_NewPointerObj((void *) P, SWIGTYPE_p_VEC, 0));
	return resultobj;
}

%}


#endif
