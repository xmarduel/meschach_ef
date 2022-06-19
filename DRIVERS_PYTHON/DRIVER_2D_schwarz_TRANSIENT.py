#! /usr/bin/env python

import sys
from math import cos, sin, sqrt, pi, exp
import time
import threading
import profile

from meschach          import *
from meschach_adds     import *
from meschach_ef       import *
from meschach_vogle    import *

#-------------------------------------------------------------------
#
# see "Finite Elements Methods"  H.R. Schwarz
#
# Chapter 6, p. 369
#
# The non-stationnary problem is solved and gives perfect results
#
#-------------------------------------------------------------------
#
#                         ref = 2
#                    *-------------*
#                    !             !
#          ref = 1   !             ! ref = 2
#                    *             *
#                    !          /
#                    !       /
#                    !     !
#                    !     ! ref = 3
#                    !     !
#       ref = 2      !      \
#                    !          \
#                    !             *
#                    !             ! ref = 2
#                    !             !
#                    *-------------*
#                      ref = 2
#
# There is a mix of BC:
# - Dirichlet (ref = 1)
# - Neumann (ref = 2)
# - Cauchy (ref = 3)
#
# so this is a good example
#
#-------------------------------------------------------------------

mem_attach_list1()
mem_attach_list2()
mem_attach_list3()
mem_attach_list4()
mem_attach_list5()
mem_attach_list6()
mem_attach_list7()

err_attach_list2()
err_attach_list3()
err_attach_list4()
err_attach_list5()
err_attach_list6()
err_attach_list7()

set_err_flag(EF_JUMP)


def init_condition(x,y):
    return 0.0

def source(x,y,t):
    return 20.0
def bc_dirichlet(x,y):
    return 0.0
def bc_neumann(x,y):
    return 0.0
def bc_cauchy(x,y):
    return 2.0

MyParams = Params_get()

#-------------------------------------------------------------------   
# LECTURE  IN  INPUT FILE "INPUT_PDE.dat"
# ECRITURE IN OUTOUT FILE "OUTPUT_PDE.dat"
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "main_problem", "NULL", "LAPLACIAN" )
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "finite_elements_params", "name_ef", "P1" )    # Type d'EF : "P1","P2", "P3" , "H3", "S3", "S5"

Params_set_oneparam(MyParams, "matrix_solver_params", "resolution_method", "CG" )     # Methode : DIRECT-METHOD,CG,CGS,GMRES(k)
Params_set_oneparam(MyParams, "matrix_solver_params", "preconditionning", "NULL" )   # Precond : NULL, ICH, ILU, DIAG.

#-physical parameters------------------------------------------------------------------

Params_set_oneparam(MyParams, "physical_params", "kappa",    1.0 ) # kappa
Params_set_oneparam(MyParams, "physical_params", "sigma",    0.0 ) # sigma
Params_set_oneparam(MyParams, "physical_params", "epsilon",  0.0 ) # epsilon  -k*LAPLACIEN(u) + e.BILAPL(u) + sigma.u =F

#-time parameters------------------------------------------------------------------

Params_set_oneparam(MyParams, "time_params", "TPS_INI",    0.0  )
Params_set_oneparam(MyParams, "time_params", "TPS_FIN",    100.0  )
Params_set_oneparam(MyParams, "time_params", "DT",         0.10  )

#- geometry-----------------------------------------------------------------------------

Params_set_oneparam(MyParams, "geometry_params", "meshfile", "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/2D/schwarz.msh" )  #  Mesh File ("name.dat")
Params_set_oneparam(MyParams, "geometry_params", "meshtype", "gmsh" )  #  Mesh File ("name.dat")
Params_set_oneparam(MyParams, "geometry_params", "meshname", "" )  #  Mesh File ("name.dat")


Params_set_oneparam(MyParams, "graphics_output_params", "GNUPLOT",     0) # GNUPLOT
Params_set_oneparam(MyParams, "graphics_output_params", "GRAPH",       0) # GRAPH
Params_set_oneparam(MyParams, "graphics_output_params", "MATLAB",      0) # MATLAB
Params_set_oneparam(MyParams, "graphics_output_params", "VTK",         0) # VTK
Params_set_oneparam(MyParams, "graphics_output_params", "SILO",        0) # SILO
Params_set_oneparam(MyParams, "graphics_output_params", "PGPLOT",      0) # PGPLOT
Params_set_oneparam(MyParams, "graphics_output_params", "VOGLE",       1) # VOGLE
Params_set_oneparam(MyParams, "graphics_output_params", "LIBSCIPLOT",  0) # LIBSCIPLOT   (0=NO1=YES)

Params_set_oneparam(MyParams, "miscellaneous_params", "itprint",      4) # ITPRINT
Params_set_oneparam(MyParams, "miscellaneous_params", "itsave",       1) # ITSAVE
Params_set_oneparam(MyParams, "miscellaneous_params", "itstat",       1) # ITSTAT
Params_set_oneparam(MyParams, "miscellaneous_params", "itgraph",      1) # ITGRAPH

#-parameter for the resolution of the linear system------------------------------------

Params_set_oneparam(MyParams, "matrix_solver_params", "max_steps", 500   ) # max_iter Nombre max d'iter de la methode iterative
Params_set_oneparam(MyParams, "matrix_solver_params", "eps_steps", 1.e-12) # eps_iter arret des iterations "  "  "   "

Params_set_oneparam(MyParams, "matrix_solver_params", "iter_info", ITER_INFOe_ALL   )  # iter_info (0 , 1=stdout, 2=file iter_file)
Params_set_oneparam(MyParams, "matrix_solver_params", "iter_file", "the_residuals.dat"  ) # iter_file le nom du fichier

Params_set_oneparam(MyParams, "matrix_solver_params", "bandwidth_method", BANDWRe_MESCHACH) # USE BAND WIDTH REDUCTION BANDWRe_BAND
Params_set_oneparam(MyParams, "matrix_solver_params", "bandwidth_option", BANDWRe_BAND)   # USE BAND WIDTH REDUCTION

#-parameter for plotting------------------------------------

Params_set_oneparam(MyParams, "graphics_interactiv2D_params", "NX", 1  )
Params_set_oneparam(MyParams, "graphics_interactiv2D_params", "NY", 1  )

Params_set_oneparam(MyParams, "graphics_interactiv2D_params", "DRIVER", "/XWINDOW"  )
Params_set_oneparam(MyParams, "graphics_interactiv2D_params", "SIZE_WINDOW_X", 700 )
Params_set_oneparam(MyParams, "graphics_interactiv2D_params", "SIZE_WINDOW_Y", 700 )
Params_set_oneparam(MyParams, "graphics_interactiv2D_params", "SIZE_LABELS", 1.1  )

Params_set_oneparam2(MyParams, "graphics_interactiv2D_params", "LABEL_LEGEND", 1,1, "Solution"  )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params", "GRAPHICS_TYPE", 1,1, "CONTOURS"  )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params", "WINDOW_X_MIN", 1,1,  0.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params", "WINDOW_X_MAX", 1,1, 10.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params", "WINDOW_Y_MIN", 1,1,  0.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params", "WINDOW_Y_MAX", 1,1, 10.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params", "NB_LEVELS"   , 1,1, 7    )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params", "LEVELS"      , 1,1, [10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0, 120.0, 130.0, 140.0, 150.0]  )


Params_set_staticparam(MyParams, 0)
#############################################################################################

MyRhsFun = Rhs2D_get()
Rhs2D_setFunctionTransientPython(MyRhsFun, 0, AXEe_X, source)
 
MyBC = Bc2D_get()

Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_X )
Bc2D_setBcType(MyBC, BC_2De_NEUMANN,   2, AXEe_X )
Bc2D_setBcType(MyBC, BC_2De_CAUCHY,    3, AXEe_X )

Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_X, bc_dirichlet )
Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN,   2, AXEe_X, bc_neumann )
Bc2D_setFunctionPython(MyBC, BC_2De_CAUCHY,    3, AXEe_X, bc_cauchy )

#############################################################################################

MyElt  = elt2D_get(Params_get_oneparam(MyParams, "finite_elements_params", "name_ef"))

#--------------------------------------------------------------------

MyGeom = Geom2D_get(MyElt,
        Params_get_oneparam(MyParams, "geometry_params", "meshfile"),
        Params_get_oneparam(MyParams, "geometry_params", "meshname"),
        Params_get_oneparam(MyParams, "geometry_params", "meshtype"))

Geom2D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X)  

NBSOMM = GEOM_2D_NBSOMM_get(MyGeom)

#--------------------------------------------------------------------

DT      = Params_get_oneparam(MyParams, "time_params", "DT")
TPS_FIN = Params_get_oneparam(MyParams, "time_params", "TPS_FIN")

#--------------------------------------------------------------------

MyIC = Fun2D_get()
Fun2D_setFunctionPython(MyIC, init_condition ) # ref_e=0


DIFF = v_get(NBSOMM)

# initial solution
U_o = v_get(NBSOMM)
U_o = build_vec_ef_from_function2D(MyElt, MyGeom, MyIC, U_o)

# steady state solution
fp = open("schwarz.dat", "r")
U_steadystate = v_finput(fp, None)
fp.close()

if VEC_dim_get(U_steadystate) == NBSOMM :
    print("exact solution from file seems OK")
else:
    print("wrong exact solution from file ... take ZERO vector: dim = %d  but geom NBSOMM = %d" % (VEC_dim_get(U_steadystate), NBSOMM))
    U_steadystate = v_get(NBSOMM)
    v_zero(U_steadystate)

# diff to steady state solution
U_diff = v_get(NBSOMM)
U_diff = v_sub(U_o, U_steadystate, U_diff)
diff = v_norm2(U_diff)
print("time = " , 0.0 , " --> ||sol_steadystate - sol_initial|| = " , diff)

#--------------------------------------------------------------------

MASS    = sp_get(NBSOMM,NBSOMM, 5)
STIFF1  = sp_get(NBSOMM,NBSOMM, 5)
MASS_BC = sp_get(NBSOMM,NBSOMM, 5)

MASS    = assemblage2D_matrix_Mass  ( MyElt , MyGeom , MASS   )
STIFF1  = assemblage2D_matrix_Stiff1( MyElt , MyGeom , STIFF1 )
MASS_BC = assemblage2D_matrix_fromBC( MyElt , MyGeom , MyBC, MASS_BC )

M_DT_STIFF = sp_get(NBSOMM,NBSOMM, 5)
sp_zero(M_DT_STIFF)
M_DT_STIFF = sp_mltadd(M_DT_STIFF, STIFF1, -DT , M_DT_STIFF)
M_DT_STIFF = sp_mltadd(M_DT_STIFF, MASS_BC, -DT , M_DT_STIFF)


# B = MASSE + DT/2(STIFF1 + MASS_BC)
B  = sp_get(NBSOMM,NBSOMM, 5)
sp_zero(B)

B = sp_mltadd( B, MASS   , 1.0    , B)
B = sp_mltadd( B, STIFF1 , 0.5*DT , B)
B = sp_mltadd( B, MASS_BC, 0.5*DT , B)

# A = MASSE - DT/2(STIFF1 + MASS_BC) for the rhs
A  = sp_get(NBSOMM,NBSOMM, 5)
sp_zero(A)

A = sp_mltadd( A, MASS   ,  1.0    , A)
A = sp_mltadd( A, STIFF1 , -0.5*DT , A)
A = sp_mltadd( A, MASS_BC, -0.5*DT , A)

# On sauvegarde B dans B_REF pour les CL sur le 2nd membre plus tard
B_REF = sp_copy( sp_compact(B,0.0) )
# On sauvegarde B dans B_REF pour les CL sur le 2nd membre plus tard
A_REF = sp_copy( sp_compact(A,0.0) )

# B.x = A.xn + d
transform2D_matrix_with_bc( MyElt , MyGeom , MyBC , B )



RHS         = v_get(NBSOMM)
RHS_FUN     = v_get(NBSOMM)
RHS_TDISCR  = v_get(NBSOMM)
RHS_BC      = v_get(NBSOMM)

RHS_FUN1    = v_get(NBSOMM)
RHS_FUN2    = v_get(NBSOMM)

RHS_BC = assemblage2D_vector_fromBC(MyElt , MyGeom , MyBC, RHS_BC)


# choleski factorisation of A
LLT = sp_copy(B)

spCHfactor(LLT)


U_nm1 = v_get(NBSOMM)
U_n   = v_get(NBSOMM)

U_nm1 = v_copy( U_o, U_nm1 )

		  
graphics1D_vopl_initialize(Params_get_oneparam(MyParams,"graphics_interactiv2D_params","DRIVER"),
						Params_get_oneparam(MyParams,"graphics_interactiv2D_params","SIZE_WINDOW_X"),
						Params_get_oneparam(MyParams,"graphics_interactiv2D_params","SIZE_WINDOW_Y"),
						1, 1, VOPL_CONTOURS)
graphics1D_vopl_title("SOLUTION")
graphics1D_vopl_window(1, 1, Params_get_oneparam2(MyParams,"graphics_interactiv2D_params","WINDOW_X_MIN",1,1),
						Params_get_oneparam2(MyParams,"graphics_interactiv2D_params","WINDOW_X_MAX",1,1),
						Params_get_oneparam2(MyParams,"graphics_interactiv2D_params","WINDOW_Y_MIN",1,1),
						Params_get_oneparam2(MyParams,"graphics_interactiv2D_params","WINDOW_Y_MAX",1,1))
			
graphics1D_vopl_nblevels(1, 1, Params_get_oneparam2(MyParams,"graphics_interactiv2D_params","NB_LEVELS",1,1))
graphics1D_vopl_levels(1, 1, Params_get_oneparam2(MyParams,"graphics_interactiv2D_params","LEVELS",1,1))
graphics1D_vopl_contourmeshdata(1, 1, GEOM_2D_NSELMT_get(MyGeom), GEOM_2D_XYSOMM_get(MyGeom))
graphics1D_vopl_contourplotdata(1, 1, U_o)

t1 = threading.Thread( target=Py_vopl_contours_with_xt_toolkit_transient )
t1.start()
#graphics1D_vopl_plotcontours_start()
			
#---------------------------------------------------------------------

atime_start = time.time()

itstat = Params_get_oneparam(MyParams, "miscellaneous_params", "itstat")
itgraph = Params_get_oneparam(MyParams, "miscellaneous_params", "itgraph")

NBITER = int( TPS_FIN / DT )

for k in range(1,NBITER+1):

    TPS    = k*DT
	 
    #print("k = %d => TIME = %lf" % (k, TPS))
	 
    # set the curent time in the functions structs
    Bc2D_setTps (MyBC, TPS)
    
    Rhs2D_setTps( MyRhsFun, TPS - DT)
    RHS_FUN1    = assemblage2D_vector_fun( MyElt , MyGeom , MyRhsFun, RHS_FUN1 )
    Rhs2D_setTps( MyRhsFun, TPS )
    RHS_FUN2    = assemblage2D_vector_fun( MyElt , MyGeom , MyRhsFun, RHS_FUN2 )
	 
    v_zero(RHS_FUN)
    RHS_FUN     = v_mltadd(RHS_FUN, RHS_FUN1, 0.5, RHS_FUN)
    RHS_FUN     = v_mltadd(RHS_FUN, RHS_FUN2, 0.5, RHS_FUN)
    RHS_FUN     = v_mltadd(RHS_FUN, RHS_BC  , 1.0, RHS_FUN)
	 
	 
    RHS_TDISCR = sp_mv_mlt(A, U_nm1, RHS_TDISCR)

    # init RHS
    v_zero(RHS)
    # set it up
    RHS = v_mltadd(RHS, RHS_FUN, DT, RHS)
    RHS = v_add(RHS, RHS_TDISCR, RHS)

    transform2D_vector_with_bc (MyElt, MyGeom, MyBC, B_REF, RHS)
    
	 
    # -- Calcul de U_np1 -----------------
    U_n =  spCHsolve(LLT, RHS, U_n)
		
    #-----------------------  statistics -------------------------
    if itstat > 0 and k % itstat == 0 :
		  
        U_diff = v_sub(U_n, U_steadystate, U_diff)
        diff = v_norm2(U_diff)
        print("time = %5.2f    --> ||sol_steadystate - sol_approchee|| = %lf" % (TPS, diff))

    #-----------------------  plot -------------------------
    if itgraph > 0 and k % itgraph == 0 :

        title = "SOL_%010.5lf" % TPS
	     
        graphics1D_vopl_contourtransientbuildinfo(1,1)
        graphics1D_vopl_contourplotdata(1, 1, U_n)
        graphics1D_vopl_title(title)
					
        graphics1D_vopl_plotcontours_flush()

    #--------------------------------------------------------------------

    v_copy(U_n, U_nm1)


atime_end = time.time()

print(">>>>>>> TOTAL TIME = ", atime_end - atime_start)


# free memory
# -----------

ELT_2D_FREE(MyElt)

GEOM_2D_FREE(MyGeom)
BC_2D_FREE(MyBC)
RHS_2D_FREE(MyRhsFun)
FUN_2D_FREE(MyIC)

PARAMS_FREE(MyParams)

#---------------------------------------------------------------------

mem_info_file(sys.stdout,0)

#mem_info_file(sys.stdout,MY_LIST1)
#mem_info_file(sys.stdout,MY_LIST2)
#mem_info_file(sys.stdout,MY_LIST3)
#mem_info_file(sys.stdout,MY_LIST4)
#mem_info_file(sys.stdout,MY_LIST5)
#mem_info_file(sys.stdout,MY_LIST6)
#mem_info_file(sys.stdout,MY_LIST7)

#----------------------------------------------------------------------
