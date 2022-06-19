#! /usr/bin/env python

import sys
import math
import time
import profile

from meschach import *
from meschach_adds import *
from meschach_ef import *
from meschach_cpgplot import *
from meschach_spooles import *

#-------------------------------------------------------------------

mem_attach_list1()
mem_attach_list2()
mem_attach_list3()
mem_attach_list4()
mem_attach_list5()
mem_attach_list6()
mem_attach_list7()

err_warn_attach_lists2()
err_warn_attach_lists3()
err_warn_attach_lists4()
err_warn_attach_lists5()
err_warn_attach_lists6()
err_warn_attach_lists7()

set_err_flag(EF_JUMP)
set_warn_flag(EF_SILENT)



def spheric_src(x,y):
    #return 0.0
    return 10.0
    return -2*y*y -2*x*x
def spheric(x,y):
    #return 0.0
    return 10.0/4.0*(0.5 - (x-0.5)*(x-0.5) - (y-0.5)*(y-0.5))
    return x*x*y*y

def bc0(x,y):
    return 0.0
def bc1(x,y):
    #return 1.0
    return 10.0/4.0*( - 2*(x-0.5) )
    return 3*x*x


MyBC = Bc2D_get()

Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_X )
Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_X )
Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 3, AXEe_X )
Bc2D_setBcType(MyBC, BC_2De_NEUMANN,   4, AXEe_X )
Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 5, AXEe_X ) # or BC_2De_NEUMANN

Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_X, spheric )
Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_X, spheric )
Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_X, spheric )
Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 4, AXEe_X, spheric )
Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 5, AXEe_X, spheric )

Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 1, AXEe_X, bc0 )
Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 2, AXEe_X, bc0 )
Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 3, AXEe_X, bc0 )
Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 4, AXEe_X, bc0 )
Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 5, AXEe_X, bc0 )


MyRhsFun = Rhs2D_get()
Rhs2D_setFunctionPython(MyRhsFun, 0, AXEe_X, spheric_src) # ref_e=0, axe=1
Rhs2D_setFunctionPython(MyRhsFun, 0, AXEe_Y, spheric_src) # ref_e=0, axe=2

MyAdvFun = Adv2D_get()
Adv2D_setFunctionPython(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_X, bc1) # ref_e=0, axe=1,1
Adv2D_setFunctionPython(MyAdvFun, 0, AXEe_X, AXEe_Y, AXEe_X, bc1) # ref_e=0, axe=1,2

#-------------------------------------------------------------------   

MyParams = Params_get()
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "main_problem","NULL", "HELMHOLZ" ) # CONVECTION-DIFFUSION  LAPLACIAN
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "finite_elements_params","name_ef", "P1" )    # Type d'EF : "P1","P2", "P3"

Params_set_oneparam(MyParams, "matrix_solver_params","resolution_method", "CG" )     # Methode : DIRECT-METHOD,CG,CGS,GMRES(k) 
Params_set_oneparam(MyParams, "matrix_solver_params","preconditionning", "ICH" )   # Precond : NULL, ICH, ILU, DIAG. 

#Params_set_oneparam(MyParams, "geometry_params","meshfile", "CUBE_11.emc2" )  #  Mesh File ("name.dat") 
Params_set_oneparam(MyParams, "geometry_params","meshfile", "MESH_P1.quad"  )  #  Mesh File ("name.dat")
Params_set_oneparam(MyParams, "geometry_params","meshtype", "quad1" )  # 
Params_set_oneparam(MyParams, "geometry_params","meshname", "MESH_20" )  #  

Params_set_oneparam(MyParams, "graphics_output_params","GNUPLOT",     1) # GNUPLOT
Params_set_oneparam(MyParams, "graphics_output_params","GRAPH",       0) # GRAPH
Params_set_oneparam(MyParams, "graphics_output_params","MATLAB",      0) # MATLAB
Params_set_oneparam(MyParams, "graphics_output_params","VTK",         1) # VTK
Params_set_oneparam(MyParams, "graphics_output_params","SILO",        1) # SILO
Params_set_oneparam(MyParams, "graphics_output_params","PGPLOT",      0) # PGPLOT
Params_set_oneparam(MyParams, "graphics_output_params","VOGLE",       0) # VOGLE
Params_set_oneparam(MyParams, "graphics_output_params","LIBSCIPLOT",  0) # LIBSCIPLOT   (0=NO;1=YES)

Params_set_oneparam(MyParams, "miscellaneous_graphics","ispmatpattern", 0 ) # IHBF
Params_set_oneparam(MyParams, "miscellaneous_graphics","igeomview",    1 ) # IHBF


#-physical parameters------------------------------------------------------------------

Params_set_oneparam(MyParams, "physical_params","epsilon",    0.0000 ) # epsilon
Params_set_oneparam(MyParams, "physical_params","sigma",      1.0    ) # sigma
Params_set_oneparam(MyParams, "physical_params","kappa",      1.000  ) # kappa  -k*LAPLACIEN(u)+ e.BILAPL(u) +sigma.u =F

Params_set_oneparam(MyParams, "convective_terms_params","delta",    0.20) # delta
Params_set_oneparam(MyParams, "convective_terms_params","method", "SUPG") # method stab (SUPG,GALS,DWG)

#-parameter for the resolution of the linear system------------------------------------

Params_set_oneparam(MyParams, "matrix_solver_params","max_steps",  500  ) # max_iter Nombre max d'iter de la methode iterative
Params_set_oneparam(MyParams, "matrix_solver_params","eps_steps", 1.e-12) # eps_iter arret des iterations "  "  "   "

Params_set_oneparam(MyParams, "miscellaneous_params","iter_info", ITER_INFOe_ALL )       # iter_info (0 , 1=stdout, 2=file iter_file)
Params_set_oneparam(MyParams, "miscellaneous_params","iter_file", "the_residuals.dat"  ) # iter_file le nom du fichier

Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_AMD)       # BANDWRe_MESCHACH BANDWRe_SPOOLES
Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_option", BANDWRe_MESCHACH_PROFILE)       # USE BAND WIDTH REDUCTION


Params_set_staticparam(MyParams, 0)


MyElt  = elt2D_get(Params_get_oneparam(MyParams, "finite_elements_params", "name_ef"))

elt_P1 = elt2D_get("P1")
elt_P2 = elt2D_get("P2")
elt_P3 = elt2D_get("P3")

#--------------------------------------------------------------------

MyGeom = Geom2D_get(MyElt, 
        Params_get_oneparam(MyParams, "geometry_params", "meshfile"),
        Params_get_oneparam(MyParams, "geometry_params", "meshname"),
        Params_get_oneparam(MyParams, "geometry_params", "meshtype"))

graphics_geom2D_view("X11", MyGeom, "dummy")
#graphics_geom2D_view("gnuplot", MyGeom, "geometry_view")


Geom2D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X)  

#Geom2D_foutput(sys.stdout, MyGeom)  

# --------------------------------------------------------------------
# --------------------------------------------------------------------

def Py_solve2D_eigen( MyElt , MyGeom , MyBC ) :
   
    NBSOMM = GEOM_2D_NBSOMM_get(MyGeom)
    
    ResolutionMethod = Params_get_oneparam(MyParams,"matrix_solver_params","resolution_method")
    Preconditionning = Params_get_oneparam(MyParams,"matrix_solver_params","preconditionning")
    print("ResolutionMethod =", ResolutionMethod)
    print("Preconditionning =", Preconditionning)
    
    A  = sp_get(NBSOMM,NBSOMM,10)
    
    # ----- assemblage matrix and rhs ------ 
    
    A = assemblage2D_matrix_Stiff1( MyElt , MyGeom , A )
    
    transform2D_matrix_with_bc( MyElt , MyGeom , MyBC , A )
    
    # ------ solve the system Ax = k.x  ----- 
    
    B = m_get(NBSOMM,NBSOMM)
    T = m_get(NBSOMM,NBSOMM)
    Q = m_get(NBSOMM,NBSOMM)
    
    B = sp_m2dense(A,B)
    T = m_copy(B,T)
    
    # schur form B = Q.T.Q'
    schur(T,Q)
    
    # eigenvalues
    eigen_re = v_get(NBSOMM)
    eigen_im = v_get(NBSOMM)
    
    schur_evals(T, eigen_re, eigen_im)
    
    # k-th eigenvector is k-th colunm of (X_re + i.X_im)
    X_re = m_get(NBSOMM,NBSOMM)
    X_im = m_get(NBSOMM,NBSOMM)
    
    schur_vecs(T,Q, X_re, X_im)
    
    # finito
    return (X_re, X_im, eigen_re, eigen_im)

# ---------------------------------------------------------------------------------------

PROBLEM = Params_get_oneparam(MyParams, "main_problem", "NULL")
print("PROBLEM is ",PROBLEM)


(X_re, X_im, eigen_re, eigen_im) = Py_solve2D_eigen( MyElt , MyGeom , MyBC )


#----------------------- graphics output -----------------------------

NBSOMM = GEOM_2D_NBSOMM_get(MyGeom)

SOL = v_get(NBSOMM)

v_foutput(sys.stdout, eigen_re)
v_foutput(sys.stdout, eigen_im)

for k in range(NBSOMM):

    #extract k-th column
    get_col(X_re, k, SOL)
    
    # title
    tmp_title = "EigenVec_%d_" %(k)
    
    graphics2D( "gnuplot" , MyElt , MyGeom , SOL , tmp_title)
    #graphics2D( "silo"    , MyElt , MyGeom , SOL , "SolApproch")
    #graphics2D( "vtk"     , MyElt , MyGeom , SOL , "SolApproch")

#---------------------------------------------------------------------

#
# free memory
# -----------

ELT_2D_FREE(MyElt)

ELT_2D_FREE(elt_P1)
ELT_2D_FREE(elt_P2)
ELT_2D_FREE(elt_P3)

GEOM_2D_FREE(MyGeom)
BC_2D_FREE(MyBC)
RHS_2D_FREE(MyRhsFun)
ADV_2D_FREE(MyAdvFun)

PARAMS_FREE(MyParams)

#---------------------------------------------------------------------

mem_info_file(sys.stdout,0)
#
#mem_info_file(sys.stdout,MY_LIST1)
#mem_info_file(sys.stdout,MY_LIST2) 
#mem_info_file(sys.stdout,MY_LIST3) 
#mem_info_file(sys.stdout,MY_LIST4) 
#mem_info_file(sys.stdout,MY_LIST5) 
#mem_info_file(sys.stdout,MY_LIST6)
#mem_info_file(sys.stdout,MY_LIST7) 

#----------------------------------------------------------------------
