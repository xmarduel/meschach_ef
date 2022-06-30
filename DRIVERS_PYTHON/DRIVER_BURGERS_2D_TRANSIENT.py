#! /usr/bin/env python

import sys
import math
import time

from meschach          import *
from meschach_adds     import *
from meschach_ef       import *
from meschach_cpgplot  import *


# TEST : U(x,t) = cos(x-t) -> Ux  = -sin(x-t)
#                             Uxx = -cos(x-t)
#                             Ut  =  sin(x-t)
#
#  Ut - k.Uxx + UUx = f(x,t)  ( kappa = 1.0  ) 

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


def U(x,t):
    return  cos(x-t)
def Ux(x,t):
    return -sin(x-t)
def Ut(x,t):
    return  sin(x-t)
def Uxx(x,t):
    return -cos(x-t)
def Uxxxx(x,t):
    return  cos(x-t)

eps = 0.0
kap = 1.0
def source(x,t):
    #return   Ut(x,t) - kap*Uxx(x,t) + U(x,t)*Ux(x,t) + eps*Uxxxx(x,t)
    return ( math.sin(x-t) - kap*(-math.cos(x-t)) - math.cos(x-t)*math.sin(x-t) + eps*math.cos(x-t) )
    return 0.0


fun1Dex = Fun1D_get()
Fun1D_setFunctionTransientPython(fun1Dex, U)


def init_condition(x):
    return U(x,0)
def Dinit_condition(x):
    return Ux(x,0)

def bc_left(x,t):
    return init_condition(x)
    return math.cos(x-t)

def bc_right(x,t):
    return init_condition(x)
    return math.cos(x-t)



READ_PARAMS_FROM_FILE = 0

MyParams = Params_get()

if READ_PARAMS_FROM_FILE == 0 :

    #-------------------------------------------------------------------   
    # LECTURE  IN  INPUT FILE "INPUT_PDE.dat"
    # ECRITURE IN OUTOUT FILE "OUTPUT_PDE.dat"
    #--------------------------------------------------------------------------------------
    Params_set_oneparam(MyParams, "main_problem","NULL", "Convection-Diffusion" )
    #--------------------------------------------------------------------------------------
    Params_set_oneparam(MyParams, "finite_elements_params","name_ef", "S3" )    # Type d'EF : "P1","P2", "P3" , "H3", "S3", "S5"
    
    Params_set_oneparam(MyParams, "matrix_solver_params","resolution_method", "DIRECT-METHOD" )     # Methode : DIRECT-METHOD,CG,CGS,GMRES(k) 
    Params_set_oneparam(MyParams, "matrix_solver_params","preconditionning", "NULL" )   # Precond : NULL, ICH, ILU
    
    #-physical parameters------------------------------------------------------------------
    
    Params_set_oneparam(MyParams, "physical_params","kappa",    0.11 ) # kappa
    Params_set_oneparam(MyParams, "physical_params","sigma",    0.0 ) # sigma
    Params_set_oneparam(MyParams, "physical_params","epsilon",  0.0 ) # epsilon  -k*LAPLACIEN(u) + e.BILAPL(u) + sigma.u =F
    
    #-time parameters------------------------------------------------------------------
    
    Params_set_oneparam(MyParams, "time_params","TPS_INI",    0.0  ) # 
    Params_set_oneparam(MyParams, "time_params","TPS_FIN",    65.0  ) # 
    Params_set_oneparam(MyParams, "time_params","DT",         0.001 ) # 
    
    #- geometry-----------------------------------------------------------------------------
    
    Params_set_oneparam(MyParams, "geometry_params","meshfile", "NULL" )     # meshfile
    Params_set_oneparam(MyParams, "geometry_params","meshname", "NULL" )     # meshname
	 Params_set_oneparam(MyParams, "geometry_params","meshtype", "line" )     #
    #
    Params_set_oneparam(MyParams, "geometry_params","xmin"  , 0.0  ) # xmax
    Params_set_oneparam(MyParams, "geometry_params","xmax"  , 2*math.pi ) # xmax
    Params_set_oneparam(MyParams, "geometry_params","nx"    ,  100 ) # nx
	 Params_set_oneparam(MyParams, "geometry_params","distribution" , "UNIFORM")
	 Params_set_oneparam(MyParams, "geometry_params","periodicity" , NON_PERIODIC_MESHe)
    
    Params_set_oneparam(MyParams, "graphics_output_params","GNUPLOT",     0) # GNUPLOT
    Params_set_oneparam(MyParams, "graphics_output_params","MATLAB",      0) # MATLAB
	 Params_set_oneparam(MyParams, "graphics_output_params","VTK",         0) # VTK
    Params_set_oneparam(MyParams, "graphics_output_params","SILO",        0) # SILO

    Params_set_oneparam(MyParams, "miscellaneous_params","itprint",      4) # ITPRINT
    Params_set_oneparam(MyParams, "miscellaneous_params","itsave",       1) # ITSAVE
    Params_set_oneparam(MyParams, "miscellaneous_params","itstat",      -1) # ITSTAT
    Params_set_oneparam(MyParams, "miscellaneous_params","itgraph",      1) # ITGRAPH

    #-parameter for the resolution of the linear system------------------------------------

    Params_set_oneparam(MyParams, "matrix_solver_params","max_steps", 15  ) # max_iter Nombre max d'iter de la methode iterative
    Params_set_oneparam(MyParams, "matrix_solver_params","eps_steps", 1.e-12) # eps_iter arret des iterations "  "  "   "

    Params_set_oneparam(MyParams, "miscellaneous_params","iter_info", ITER_INFOe_ALL   )  # iter_info (0 , 1=stdout, 2=file iter_file)
    Params_set_oneparam(MyParams, "miscellaneous_params","iter_file", "the_residuals.dat"  ) # iter_file le nom du fichier
    #

    #-parameter for plotting------------------------------------

    Params_set_oneparam(MyParams, "graphics_curveplots_params", "DRIVER" , "/XWINDOW" )  #"/XWINDOW"  "/PPM"  "/GIF"  

    Params_set_oneparam(MyParams, "graphics_curveplots_params", "SIZE_WINDOW_X", 700) 
    Params_set_oneparam(MyParams, "graphics_curveplots_params", "SIZE_WINDOW_Y", 500) 

    Params_set_oneparam(MyParams, "graphics_curveplots_params", "SIZE_LABELS", 2.0 )

    Params_set_oneparam(MyParams, "graphics_curveplots_params", "NX", 2  ) 
    Params_set_oneparam(MyParams, "graphics_curveplots_params", "NY", 2  ) 

    #Params_set_oneparam(MyParams, "graphics_curveplots_params", "CURSOR", 1  ) 

    Params_set_oneparam2(MyParams, "graphics_curveplots_params","LABEL_LEGEND", 1,1, "U(x,t)") 
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","LABEL_AXE_X" , 1,1, "(x)") 
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","LABEL_AXE_Y" , 1,1, "(y)") 
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","MARKER"      , 1,1, 0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","MARKER_END"  , 1,1, 0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_X_MIN", 1,1,  0.0  ) 
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_X_MAX", 1,1,  2*math.pi )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_Y_MIN", 1,1, -2.0  ) 
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_Y_MAX", 1,1,  2.0  ) 

    Params_set_oneparam2(MyParams, "graphics_curveplots_params","LABEL_LEGEND", 2,1, "KS - L2 Norm of Sol")
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","LABEL_AXE_X" , 2,1, "(t)")
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","LABEL_AXE_Y" , 2,1, "|U(x,t)|")
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","MARKER"      , 2,1, 0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","MARKER_END"  , 2,1, 0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_X_MIN", 2,1,    0.0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_X_MAX", 2,1,   100.0 )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_Y_MIN", 2,1,    0.0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_Y_MAX", 2,1,   12.0  )

    Params_set_oneparam2(MyParams, "graphics_curveplots_params","LABEL_LEGEND", 1,2, "KS - L2 Norm of Sol (Zoom)")
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","LABEL_AXE_X" , 1,2, "(t)"   )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","LABEL_AXE_Y" , 1,2, "|U(x,t)|")
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","MARKER"      , 1,2, 0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","MARKER_END"  , 1,2, 0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_X_MIN", 1,2, 20.0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_X_MAX", 1,2, 30.0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_Y_MIN", 1,2,  5.0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_Y_MAX", 1,2, 11.0  )

    Params_set_oneparam2(MyParams, "graphics_curveplots_params","LABEL_LEGEND", 2,2, "KS - Phase Plane representation")
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","LABEL_AXE_X" , 2,2, "|U|")
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","LABEL_AXE_Y" , 2,2, "d/dt |U|")
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","MARKER"      , 2,2, 0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","MARKER_END"  , 2,2, 6  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_X_MIN", 2,2,  6.0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_X_MAX", 2,2, 10.0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_Y_MIN", 2,2,-8.0  )
    Params_set_oneparam2(MyParams, "graphics_curveplots_params","WINDOW_Y_MAX", 2,2, 8.0  )


    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_X_MIN", 0, 1.1   )
    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_X_MAX", 0, 1.2   )
    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_Y_MIN", 0, 2.9   )
    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_Y_MAX", 0, 3.0   )
    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_COLOR", 0, PGPLOTe_COLOR_RED   )
    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_FILL" , 0, PGPLOTe_FILL_STYLE_SOLID   )
    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_LABEL", 0, "Exit" )

    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_X_MIN", 1, 1.1   )
    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_X_MAX", 1, 1.2   )
    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_Y_MIN", 1, 2.7   )
    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_Y_MAX", 1, 2.8   )
    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_COLOR", 1, PGPLOTe_COLOR_GREEN   )
    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_FILL" , 1, PGPLOTe_FILL_STYLE_HATCHED   )
    Params_set_oneparam1(MyParams, "graphics_curveplots_params","BOX_LABEL", 1, "Run" )


if READ_PARAMS_FROM_FILE == 0 :

    MyIC = Fun1D_get()
    MyDIC = Fun1D_get()
    Fun1D_setFunctionPython(MyIC , init_condition ) # ref_e=0
    Fun1D_setFunctionPython(MyDIC, Dinit_condition ) # ref_e=0
    
    
    MyBC = Bc1D_get()
    Bc1D_setFunctionTransientPython1(MyBC, BC_1De_DIRICHLET, 1, AXEe_X, bc_left ) # ref_s=1 (left) 
    Bc1D_setFunctionTransientPython1(MyBC, BC_1De_DIRICHLET, 2, AXEe_X, bc_right) # ref_s=2 (right)   
    
    Bc1D_setMaskValue(MyBC, BC_1De_DIRICHLET, 1, AXEe_X)
    Bc1D_setMaskValue(MyBC, BC_1De_DIRICHLET, 2, AXEe_X) # BC_1De_CAUCHY, BC_1De_DIRICHLET, BC_1De_NEUMANN
    
    MyRhsFun = Rhs1D_get()
    #Rhs1D_setFunctionTransientPython(MyRhsFun, 0, AXEe_X, source ) # ref_e=0
    Rhs1D_setCFunctionTransient( MyRhsFun, 0, AXEe_X, Src1D_00_Transient )
    
    
    MyAdvFun = Adv1D_get()
    Adv1D_setFunctionTransientPython(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_X, lambda x,t: 0.0 ) # ref_e=0

else :

    # Boundary conditions
    MyBC = Bc1D_get()
    for i in range(1, Params_get_oneparam(MyParams,"boundary_conditions_params","nb_BCdirichlet")+1) :
    
        idx = Params_get_oneparam2(MyParams,"boundary_conditions_params","BcDirichlet", AXEe_X, i)
        print("idx bc dirichlet =", idx)
        Bc1D_setCFunction( MyBC, BC_1De_DIRICHLET, i, AXEe_X , Src1D_00 )
    
    
    # Right Hand Side
    MyRhsFun = Rhs1D_get()
    idx = Params_get_oneparam1(MyParams,"right_hand_side_params", "rhs", AXEe_X)
    print("idx rhs =", idx)
    Rhs1D_setCFunctionTransient( MyRhsFun, 0, AXEe_X, Src1D_00_Transient )

    # Initial Condition
    MyIC = Fun1D_get()
    idx = Params_get_oneparam1(MyParams,"initial_condition_params", "ic", AXEe_X)
    print("idx ic =", idx)
    Fun1D_setCFunctionTransient( MyIC, Src1D_07_Transient )

#
#############################################################################################
Params_set_staticparam(MyParams, 0)

MyElt  = elt1D_get(Params_get_oneparam(MyParams, "finite_elements_params", "name_ef"))

#--------------------------------------------------------------------

if ( Params_get_oneparam(MyParams, "geometry_params", "meshfile") != "NULL"):

    MyGeom = Geom1D_get_fromfile(MyElt,
        Params_get_oneparam(MyParams, "geometry_params", "meshfile"),
        Params_get_oneparam(MyParams, "geometry_params", "meshname"),
        Params_get_oneparam(MyParams, "geometry_params", "meshtype") )

else:

   MyGeom = Geom1D_get_fromscratch(MyElt,
        Params_get_oneparam(MyParams, "geometry_params", "xmin"),
        Params_get_oneparam(MyParams, "geometry_params", "xmax"),
        Params_get_oneparam(MyParams, "geometry_params", "nx"),
        Params_get_oneparam(MyParams, "geometry_params", "periodicity"),
        Params_get_oneparam(MyParams, "geometry_params", "distribution"))
   


bCheckBCwithGEOM = Geom1D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X)

#--------------------------------------------------------------------

NBSOMM = GEOM_1D_NBSOMM_get(MyGeom)

print("NBSOMM=", NBSOMM)
MASS   = sp_get(NBSOMM, NBSOMM, 5)
STIFF1 = sp_get(NBSOMM, NBSOMM, 5)

MASS   = assemblage1D_matrix_Mass  ( MyElt , MyGeom , MASS   )
STIFF1 = assemblage1D_matrix_Stiff1( MyElt , MyGeom , STIFF1 )


kappa   = Params_get_oneparam(MyParams, "physical_params","kappa")


DT      = Params_get_oneparam(MyParams, "time_params", "DT")
TPS_FIN = Params_get_oneparam(MyParams, "time_params", "TPS_FIN")

NBITER = int( TPS_FIN / DT )
print("NBITER = ", NBITER)


A  = sp_get(NBSOMM,NBSOMM, 5)
sp_zero(A)

A = sp_mltadd( A, MASS  , 1.0      , A)
A = sp_mltadd( A, STIFF1, DT*kappa , A)

# On sauvegarde A dans A_REF pour les CL sur le 2nd membre plus tard
A_REF = sp_copy( sp_compact(A,0.0) )


transform1D_matrix_with_bc( MyElt , MyGeom , MyBC , A )
# choleski factorisation of A
LLT = sp_copy(A)

spCHfactor(LLT)

# initial solution
U_o = v_get(NBSOMM)
U_o = build_vec_ef_from_function1Dtransient( MyElt, MyGeom, MyIC, MyDIC, U_o, 0.0)


U_nm2 = v_get(NBSOMM)
U_nm1 = v_get(NBSOMM)
U_n   = v_get(NBSOMM)

U_tps = v_get(NBSOMM)
U_lin = v_get(NBSOMM)
U_nli = v_get(NBSOMM)


v_copy(U_o, U_nm2)
v_copy(U_o, U_nm1)

RHS         = v_get(NBSOMM)
RHS_FUN     = v_get(NBSOMM)
RHS_TDISCR  = v_get(NBSOMM)
RHS_UUx     = v_get(NBSOMM)
RHS_LIN     = v_get(NBSOMM)

#----------------------- graphics interative -------------------------

x_left  = Params_get_oneparam(MyParams, "geometry_params","xmin" ) 
x_right = Params_get_oneparam(MyParams, "geometry_params","xmax" )
NBSOMM  = Params_get_oneparam(MyParams, "geometry_params","nx"   )

DX = ( x_right - x_left ) / NBSOMM


dim_graph = SPMAT_m_get(GEOM_1D_EF_to_WORLD_get(MyGeom))

U_graph1 = v_get(dim_graph)
U_graph2 = v_get(NBITER+2)
U_graph3 = v_get(NBITER+2)


ABSCISSASx = GEOM_1D_XSOMM_get(MyGeom)
ABSCISSAS1 = v_get(dim_graph)
if ( "H3"==Params_get_oneparam(MyParams, "finite_elements_params", "name_ef") ):
    ABSCISSAS1 = sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), ABSCISSASx, ABSCISSAS1 )
else:
    ABSCISSAS1 = v_copy(ABSCISSASx, ABSCISSAS1)

ABSCISSAS2 = v_get(NBITER+2)
for i in range(0,NBITER+2):
   v_set_val(ABSCISSAS2, i, i*DT)


if  Params_get_oneparam(MyParams, "graphics_output_params", "PGPLOT") :

   NX = Params_get_oneparam(MyParams, "graphics_curveplots_params", "NX")
   NY = Params_get_oneparam(MyParams, "graphics_curveplots_params", "NY")
   
   graphics1D_cpgplot_initialize(NX, NY)
   
   
   dim_graph = SPMAT_m_get(GEOM_1D_EF_to_WORLD_get(MyGeom))
   print("dim_graph =", dim_graph)
   
   #sp_mv_mlt(MyGeom->EF_to_WORLD, U_o, U_graph1)
   sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), U_o, U_graph1 )
   
   v_set_val(U_graph2, 0, v_norm2(U_graph1)*sqrt(DX))
   #U_graph2->ve[0] = v_norm2(U_graph1)*sqrt(DX)
   
   v_set_val(U_graph3, 0, 0.0)
   #U_graph3->ve[0] = 0.0
   
   graphics1D_cpgplot_plotcurves1(1,1, PGPLOTe_VIEWPORT_OLD,  -1,-1, PGPLOTe_COLOR_GREEN, ABSCISSAS1, U_graph1 )
   graphics1D_cpgplot_plotcurves1(2,1, PGPLOTe_VIEWPORT_OLD,   0, 1, PGPLOTe_COLOR_RED  , ABSCISSAS2, U_graph2 )
   graphics1D_cpgplot_plotcurves1(1,2, PGPLOTe_VIEWPORT_OLD,   0, 1, PGPLOTe_COLOR_RED  , ABSCISSAS2, U_graph2 )
   graphics1D_cpgplot_plotcurves1(2,2, PGPLOTe_VIEWPORT_OLD,   0, 1, PGPLOTe_COLOR_RED  , U_graph2  , U_graph3 )
   
   graphics1D_cpgplot_checkcursor()

#---------------------------------------------------------------------

atime_start = time.time()


for n in range(1,2):

    # set the cuurent time in the functions structs
    Bc1D_setTps ( MyBC    , n*DT )
    Rhs1D_setTps( MyRhsFun, n*DT )
    
    
    # term from time discretisation
    RHS_TDISCR = sp_mv_mlt( MASS, U_nm1, RHS_TDISCR)                               #  RHS_TDISCR = MASS * U_nm1

    # term from rhs function
    RHS_FUN = assemblage1D_vector_fun( MyElt, MyGeom, MyRhsFun , RHS_FUN )         #  RHS_FUN   = DT * F_n

    # term from convection
    RHS_UUx = assemblage1D_vector_abx( MyElt, MyGeom, U_nm1, U_nm1 , RHS_UUx )     #  RHS_UUx   = DT * [ U_nm1 * GRAD( U_nm1 ) ]


    v_zero(RHS)
    v_mltadd( RHS , RHS_TDISCR,  1.0 , RHS )
    v_mltadd( RHS , RHS_UUx   , -DT  , RHS )
    v_mltadd( RHS , RHS_FUN   ,  DT  , RHS )

    # finally, set the bc on the rhs
    transform1D_vector_with_bc( MyElt , MyGeom , MyBC , A_REF , RHS )

    # solve the system 
    U_n = spCHsolve( LLT, RHS, U_n )

    #-----------------------  statistics -------------------------

    itstat = Params_get_oneparam(MyParams, "miscellaneous_params", "itstat")

    if (itstat > 0) and (n % itstat) == 0 :

        sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), U_nm1, RHS )
        diff = diff_vec_function1Dtransient( MyElt, MyGeom, RHS, fun1Dex, 0.0, RHS )

        print("diff = ", diff)
        print("time = 0.0 -> diff solexacte-solapprochee = ", diff*sqrt(DX))
        print("time = 0.0 -> diff solexacte-solapprochee ** 2 = ", diff*diff*(DX))

        sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), U_n, RHS )
        diff = diff_vec_function1Dtransient( MyElt, MyGeom, RHS, fun1Dex, n*DT, RHS )
        print("time = ", (n*DT) , "-> diff solexacte-solapprochee = ", diff*sqrt(DX))
        print("time = ", (n*DT) , "-> diff solexacte-solapprochee = ", diff*diff*(DX))

    #----------------------- graphics interativ -------------------------
    
    #U_graph1 = sp_mv_mlt(MyGeom->EF_to_WORLD, U_n, U_graph1)
    sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), U_n, U_graph1 )
    #U_graph2->ve[k] = v_norm2(U_graph1)*sqrt(DX)
    v_set_val(U_graph2, n, v_norm2(U_graph1)*sqrt(DX))
    #U_graph3->ve[k] = ( U_graph2->ve[k] - U_graph2->ve[k-1] ) / DT
    v_set_val(U_graph3, n, ( v_get_val(U_graph2,n) - v_get_val(U_graph2,n-1) ) / DT)
		  

    itgraph = Params_get_oneparam(MyParams, "miscellaneous_params", "itgraph")
    
	 if (n % itgraph) == 0 :
    
        # -- Graphics with PGPLOT library ---- 
        if Params_get_oneparam(MyParams, "graphics_output_params", "PGPLOT") :
    
            k1 = n - itgraph
            k2 = n

            tmp_title = "U(x,t)  ( t = %10.5lf )" %(n*DT)

            graphics1D_cpgplot_newlabel(1,1, tmp_title)
	
            # create new pages if DRIVER = "GIF" or "PS" e.g. */
            if ( Params_get_oneparam(MyParams, "graphics_curveplots_params", "DRIVER") != "/XWINDOW" ) :    

                graphics1D_cpgplot_cpgpage()

                graphics1D_cpgplot_plotcurves1(1,1, PGPLOTe_VIEWPORT_NEW,  -1,-1, PGPLOTe_COLOR_RED   , ABSCISSAS1, U_graph1 )
                graphics1D_cpgplot_plotcurves1(2,1, PGPLOTe_VIEWPORT_NEW,   0,k2, PGPLOTe_COLOR_BLUE1 , ABSCISSAS2, U_graph2 )
                graphics1D_cpgplot_plotcurves1(1,2, PGPLOTe_VIEWPORT_NEW,   0,k2, PGPLOTe_COLOR_BLUE1 , ABSCISSAS2, U_graph2 )
                graphics1D_cpgplot_plotcurves1(2,2, PGPLOTe_VIEWPORT_NEW,   0,k2, PGPLOTe_COLOR_BLUE1 , U_graph2  , U_graph3 )

            else :

                graphics1D_cpgplot_inquirecursor()
                
                graphics1D_cpgplot_plotcurves1(1,1, PGPLOTe_VIEWPORT_NEW,  -1,-1, PGPLOTe_COLOR_RED   , ABSCISSAS1, U_graph1 )
                graphics1D_cpgplot_plotcurves1(2,1, PGPLOTe_VIEWPORT_OLD,  k1,k2, PGPLOTe_COLOR_BLUE1 , ABSCISSAS2, U_graph2 )
                graphics1D_cpgplot_plotcurves1(1,2, PGPLOTe_VIEWPORT_OLD,  k1,k2, PGPLOTe_COLOR_BLUE1 , ABSCISSAS2, U_graph2 )
                graphics1D_cpgplot_plotcurves1(2,2, PGPLOTe_VIEWPORT_OLD,  k1,k2, PGPLOTe_COLOR_BLUE1 , U_graph2  , U_graph3 )
                
                graphics1D_cpgplot_drawboxes(0)
                graphics1D_cpgplot_drawboxes(1)


            if n == NBITER-1 :
                Params_set_oneparam(MyParams, "graphics_curveplots_params", "CURSOR", 1)
            # ie at the end CURSOR is asked
            if Params_get_oneparam(MyParams, "graphics_curveplots_params", "CURSOR") :
                graphics1D_cpgplot_cpgcursor()

    #--------------------------------------------------------------------

    v_copy( U_nm1, U_nm2 )
    v_copy( U_n  , U_nm1 )




sp_zero(A)

A = sp_mltadd( A, MASS  , 1.5         , A) 
A = sp_mltadd( A, STIFF1, 0.5*DT*kappa, A)

# On sauvegarde A dans A_OLD pour les CL sur le 2nd membre
A_REF = sp_copy( sp_compact(A,0.0) )

transform1D_matrix_with_bc( MyElt , MyGeom , MyBC , A )

# choleski factorisation of A
sp_copy2(A, LLT)
spCHfactor(LLT)

for n in range(2,NBITER+1):
   
    # set the curent time in the functions structs
    Bc1D_setTps ( MyBC    , n*DT )
    Rhs1D_setTps( MyRhsFun, n*DT )
    
    
    v_zero( U_tps )
    U_tps = v_mltadd( U_tps , U_nm1 ,  2.0 , U_tps )
    U_tps = v_mltadd( U_tps , U_nm2 , -0.5 , U_tps )
    #U_tps = v_mltadd( U_tps , U_nm1 , 1.0 , U_tps )
    
    v_zero( U_lin )
    #U_lin = v_mltadd( U_lin , U_nm1 ,  1.0 , U_lin ) # explicit
    #U_lin = v_mltadd( U_lin , U_nm2 ,  0.0 , U_lin ) # implicit
    U_lin = v_mltadd( U_lin , U_nm1 ,   0.5 , U_lin ) # Crank-Nicholson
    
    v_zero( U_nli )
    U_nli = v_mltadd( U_nli , U_nm1 ,  2.0 , U_nli )
    U_nli = v_mltadd( U_nli , U_nm2 , -1.0 , U_nli )
    
    
    # term from linear operator
    RHS_LIN = sp_mv_mlt( STIFF1 , U_lin , RHS_LIN )
    
    # term from time discretisation
    RHS_TDISCR= sp_mv_mlt( MASS, U_tps, RHS_TDISCR)                                #  RHS_TDISCR = MASS * U_tps
    
    # term from rhs function
    RHS_FUN = assemblage1D_vector_fun( MyElt, MyGeom, MyRhsFun , RHS_FUN )         #  RHS_FUN   = DT * F_n
    
    # term from convection
    RHS_UUx = assemblage1D_vector_abx( MyElt, MyGeom, U_nli, U_nli , RHS_UUx )     #  RHS_UUx   = DT * [ U_nli * GRAD( U_nli ) ]
    
    
    v_zero(RHS)
    v_mltadd( RHS , RHS_TDISCR, 1.0 , RHS )
    v_mltadd( RHS , RHS_UUx   , -DT , RHS )
    v_mltadd( RHS , RHS_FUN   ,  DT , RHS )
    v_mltadd( RHS , RHS_LIN   , -DT*kappa , RHS )
    
    
    # finally, set the bc on the rhs
    transform1D_vector_with_bc( MyElt , MyGeom , MyBC , A_REF , RHS )
    
    # solve the system 
    U_n = spCHsolve( LLT, RHS, U_n )
    
    #-----------------------  statistics -------------------------
    
    itstat = Params_get_oneparam(MyParams, "miscellaneous_params", "itstat")

    if ( (itstat > 0) and (n % itstat) == 0 ):

        sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), U_n, RHS )

        diff = diff_vec_function1Dtransient( MyElt, MyGeom, RHS, fun1Dex, n*DT, RHS )
        print("time = ", (n*DT) , "-> diff solexacte-solapprochee = ", diff*sqrt(DX))
        print("time = ", (n*DT) , "-> diff solexacte-solapprochee = ", diff*diff*(DX))

    #----------------------- graphics interativ -------------------------
    
    #U_graph1 = sp_mv_mlt(MyGeom->EF_to_WORLD, U_n, U_graph1)
    sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), U_n, U_graph1 )
    #U_graph2->ve[k] = v_norm2(U_graph1)*sqrt(DX)
    v_set_val(U_graph2, n, v_norm2(U_graph1)*sqrt(DX) )
    #U_graph3->ve[k] = ( U_graph2->ve[n] - U_graph2->ve[n-1] ) / DT
    v_set_val(U_graph3, n, ( v_get_val(U_graph2,n) - v_get_val(U_graph2,n-1) ) / DT )
		  
		  
    itgraph = Params_get_oneparam(MyParams, "miscellaneous_params", "itgraph")
    
	 if (n % itgraph) == 0 :

        # -- Graphics with PGPLOT library ----
        if Params_get_oneparam(MyParams, "graphics_output_params", "PGPLOT") :

            k1 = n - itgraph
            k2 = n

            tmp_title = "U(x,t)  ( t = %10.5lf )" % (n*DT)

            graphics1D_cpgplot_newlabel(1,1, tmp_title)

            # create new pages if DRIVER = "GIF" or "PS" e.g. */
            if ( Params_get_oneparam(MyParams, "graphics_curveplots_params", "DRIVER") != "/XWINDOW" ) :    

                graphics1D_cpgplot_cpgpage()

                graphics1D_cpgplot_plotcurves1(1,1, PGPLOTe_VIEWPORT_NEW,  -1,-1, PGPLOTe_COLOR_RED   , ABSCISSAS1, U_graph1 )
                graphics1D_cpgplot_plotcurves1(2,1, PGPLOTe_VIEWPORT_NEW,   0,k2, PGPLOTe_COLOR_BLUE1 , ABSCISSAS2, U_graph2 )
                graphics1D_cpgplot_plotcurves1(1,2, PGPLOTe_VIEWPORT_NEW,   0,k2, PGPLOTe_COLOR_BLUE1 , ABSCISSAS2, U_graph2 )
                graphics1D_cpgplot_plotcurves1(2,2, PGPLOTe_VIEWPORT_NEW,   0,k2, PGPLOTe_COLOR_BLUE1 , U_graph2  , U_graph3 )

            else :

                graphics1D_cpgplot_plotcurves1(1,1, PGPLOTe_VIEWPORT_NEW,  -1,-1, PGPLOTe_COLOR_RED   , ABSCISSAS1, U_graph1 )
                graphics1D_cpgplot_plotcurves1(2,1, PGPLOTe_VIEWPORT_OLD,  k1,k2, PGPLOTe_COLOR_BLUE1 , ABSCISSAS2, U_graph2 )
                graphics1D_cpgplot_plotcurves1(1,2, PGPLOTe_VIEWPORT_OLD,  k1,k2, PGPLOTe_COLOR_BLUE1 , ABSCISSAS2, U_graph2 )
                graphics1D_cpgplot_plotcurves1(2,2, PGPLOTe_VIEWPORT_OLD,  k1,k2, PGPLOTe_COLOR_BLUE1 , U_graph2  , U_graph3 )

                graphics1D_cpgplot_drawboxes(0)
                graphics1D_cpgplot_drawboxes(1)


            if n == NBITER :
                Params_set_oneparam(MyParams, "graphics_curveplots_params", "CURSOR", 1)
            # ie at the end CURSOR is asked
            if Params_get_oneparam(MyParams, "graphics_curveplots_params", "CURSOR") :
                graphics1D_cpgplot_cpgcursor()

    #--------------------------------------------------------------------

    v_copy( U_nm1, U_nm2 )
    v_copy( U_n  , U_nm1 )


atime_end = time.time()

print(">>>>>>> TOTAL TIME = ", atime_end - atime_start)


# free memory
# -----------

ELT_2D_FREE(MyElt)

GEOM_2D_FREE(MyGeom)
BC_2D_FREE(MyBC)
RHS_2D_FREE(MyRhsFun)
ADV_2D_FREE(MyAdvFun)
FUN_2D_FREE(MyIC)

PARAMS_FREE(MyParams)

#---------------------------------------------------------------------

mem_info_file(stdout,0)

#mem_info_file(stdout,MY_LIST1)
#mem_info_file(stdout,MY_LIST2)
#mem_info_file(stdout,MY_LIST3)
#mem_info_file(stdout,MY_LIST4)
#mem_info_file(stdout,MY_LIST5)
#mem_info_file(stdout,MY_LIST6)
#mem_info_file(stdout,MY_LIST7)



#----------------------------------------------------------------------
