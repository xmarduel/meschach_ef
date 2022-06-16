
from math import *

import threading

from meschach            import *
from meschach_adds       import *
from meschach_ef         import *
from meschach_cpgplot    import *
from meschach_vogle      import *
from meschach_libsciplot import *

#
#
# TEST : U(x,t) = cos(x-t) -> Ux  = -sin(x-t)
#                             Uxx = -cos(x-t)
#                             Ut  =  sin(x-t)
#
#  Ut + e.Uxxxx - k.Uxx + UUx = f(x,t)  ( kappa = -1.0 , epsilon = 0.2 ) 
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

warn_attach_list2()
warn_attach_list3()
warn_attach_list4()
warn_attach_list5()
warn_attach_list6()
warn_attach_list7()

set_err_flag(EF_JUMP)


eps =  0.2
kap = -1.0
sig =  0.0


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
  
def source(x,t):
    return   Ut(x,t) - kap*Uxx(x,t) + U(x,t)*Ux(x,t) + eps*Uxxxx(x,t)


fun1Dex = Fun1D_get()
Fun1D_setFunctionTransientPython(fun1Dex, U) 


def init_condition(x):
    return U(x,0)
def Dinit_condition(x):
    return Ux(x,0)

def bc_left(x,t):
    return  cos(x-t)

def bc_right(x,t):
    return  cos(x-t)


fp2 = open("OUTPUT_PDE.dat", "w")

MyParams = Params_get()

#-------------------------------------------------------------------   
# LECTURE  IN  INPUT FILE "INPUT_PDE.dat"
# ECRITURE IN OUTOUT FILE "OUTPUT_PDE.dat"
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "input_output_files","fp2", fp2)
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "main_problem","NULL", "Convection-Diffusion" )
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "finite_elements_params","name_ef", "S3" )  # Type d'EF : "P1","P2", "P3" , "H3", "S3", "S5"
#
Params_set_oneparam(MyParams, "matrix_solver_params","resolution_method", "DIRECT-METHOD" )  # Methode : DIRECT-METHOD,CG,CGS,GMRES(k) 
Params_set_oneparam(MyParams, "matrix_solver_params","preconditionning", "NULL" )   # Precond : NULL, ICH, ILU, DIAG. 
#
#-physical parameters------------------------------------------------------------------
#
Params_set_oneparam(MyParams, "physical_params","kappa", kap ) # kappa
Params_set_oneparam(MyParams, "physical_params","sigma", sig ) # sigma
Params_set_oneparam(MyParams, "physical_params","epsilon", eps ) # epsilon  -k*LAPLACIEN(u) + e.BILAPL(u) + sigma.u =F
#
#-time parameters------------------------------------------------------------------
#
Params_set_oneparam(MyParams, "time_params","TPS_INI",0.0  )
Params_set_oneparam(MyParams, "time_params","TPS_FIN",   10.0  )
Params_set_oneparam(MyParams, "time_params","DT", 0.001 )
#
#- geometry-----------------------------------------------------------------------------
#
Params_set_oneparam(MyParams, "geometry_params","meshfile", "NULL" )     #  meshfile
Params_set_oneparam(MyParams, "geometry_params","meshtype", "line" )     # 	
Params_set_oneparam(MyParams, "geometry_params","meshname", "NULL" )     # 
#
Params_set_oneparam(MyParams, "geometry_params","xmin"  , 0.0  ) # xmax
Params_set_oneparam(MyParams, "geometry_params","xmax"  , 2*pi ) # xmax
Params_set_oneparam(MyParams, "geometry_params","nx"    , 100  ) # nx
Params_set_oneparam(MyParams, "geometry_params","distribution",  "UNIFORM" ) # distribution
Params_set_oneparam(MyParams, "geometry_params","periodicity", PERIODIC_MESHe )     # "non-periodic","periodic"
#
Params_set_oneparam(MyParams, "graphics_output_params","GNUPLOT",     0) # GNUPLOT
Params_set_oneparam(MyParams, "graphics_output_params","GRAPH",       0) # GRAPH
Params_set_oneparam(MyParams, "graphics_output_params","MATLAB",      0) # MATLAB
Params_set_oneparam(MyParams, "graphics_output_params","VTK",         0) # VTK
Params_set_oneparam(MyParams, "graphics_output_params","SILO",        0) # SILO
Params_set_oneparam(MyParams, "graphics_output_params","PGPLOT",      1) # PGPLOT
Params_set_oneparam(MyParams, "graphics_output_params","VOGLE",       0) # VOGLE
Params_set_oneparam(MyParams, "graphics_output_params","LIBSCIPLOT",  0) # LIBSCIPLOT
#
Params_set_oneparam(MyParams, "miscellaneous_params","itprint",     10) # ITPRINT
Params_set_oneparam(MyParams, "miscellaneous_params","itsave",       1) # ITSAVE
Params_set_oneparam(MyParams, "miscellaneous_params","itstat",      50) # ITSTAT
Params_set_oneparam(MyParams, "miscellaneous_params","itgraph",     10) # ITGRAPH
#
#-parameter for the resolution of the linear system------------------------------------
#
Params_set_oneparam(MyParams, "matrix_solver_params","max_steps", 15  ) # max_iter Nombre max d'iter de la methode iterative
Params_set_oneparam(MyParams, "matrix_solver_params","eps_steps", 1.e-12) # eps_iter arret des iterations "  "  "   "
#
Params_set_oneparam(MyParams, "matrix_solver_params","iter_info", ITER_INFOe_ALL   )  # iter_info (0 , 1=stdout, 2=file iter_file)
Params_set_oneparam(MyParams, "matrix_solver_params","iter_file", "the_residuals.dat"  ) # iter_file le nom du fichier
#
#
#-parameter for plotting------------------------------------
#
Params_set_oneparam(MyParams, "graphics_interactiv1D_params", "DRIVER" , "/XWINDOW" )
#
Params_set_oneparam(MyParams, "graphics_interactiv1D_params", "SIZE_WINDOW_X", 700)
Params_set_oneparam(MyParams, "graphics_interactiv1D_params", "SIZE_WINDOW_Y", 500)
#
Params_set_oneparam(MyParams, "graphics_interactiv1D_params", "NX", 2  )
Params_set_oneparam(MyParams, "graphics_interactiv1D_params", "NY", 2  )
#
Params_set_oneparam(MyParams, "graphics_interactiv1D_params", "SIZE_LABELS", 1.5 )
#
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","LABEL_LEGEND", 1,1, "U(x,t)")
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","LABEL_AXE_X" , 1,1, "(x)")
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","LABEL_AXE_Y" , 1,1, "(y)")
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","MARKER"      , 1,1, 0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","MARKER_END"  , 1,1, 0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_X_MIN", 1,1,  0.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_X_MAX", 1,1,  2*pi )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_Y_MIN", 1,1, -2.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_Y_MAX", 1,1,  2.0  )
#
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","LABEL_LEGEND", 2,1, "KS - L2 Norm of Sol")
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","LABEL_AXE_X" , 2,1, "(t)")
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","LABEL_AXE_Y" , 2,1, "|U(x,t)|")
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","MARKER"      , 2,1, 0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","MARKER_END"  , 2,1, 0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_X_MIN", 2,1,    0.0 )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_X_MAX", 2,1,  10.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_Y_MIN", 2,1,    0.0 )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_Y_MAX", 2,1,   12.0 )
#
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","LABEL_LEGEND", 1,2, "KS - L2 Norm of Sol (Zoom)")
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","LABEL_AXE_X" , 1,2, "(t)")
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","LABEL_AXE_Y" , 1,2, "|U(x,t)|")
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","MARKER"      , 1,2, 0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","MARKER_END"  , 1,2, 0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_X_MIN", 1,2, 20.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_X_MAX", 1,2, 30.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_Y_MIN", 1,2,  5.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_Y_MAX", 1,2, 12.0  )
#
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","LABEL_LEGEND", 2,2, "KS - Phase Plane representation")
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","LABEL_AXE_X" , 2,2, "|U|")
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","LABEL_AXE_Y" , 2,2, "d/dt |U|")
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","MARKER"      , 2,2, 0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","MARKER_END"  , 2,2, 6  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_X_MIN", 2,2,  6.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_X_MAX", 2,2, 10.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_Y_MIN", 2,2, -7.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv1D_params","WINDOW_Y_MAX", 2,2,  7.0  )
#
#
Params_set_staticparam(MyParams, 0)

MyIC = Fun1D_get()
MyDIC = Fun1D_get()
Fun1D_setFunctionPython(MyIC , init_condition ) # ref_e=0
Fun1D_setFunctionPython(MyDIC, Dinit_condition ) # ref_e=0


MyBC = Bc1D_get()
Bc1D_setFunctionTransientPython(MyBC, BC_1De_DIRICHLET, 1, AXEe_X, bc_left ) # ref_s=1 (left) 
Bc1D_setFunctionTransientPython(MyBC, BC_1De_DIRICHLET, 2, AXEe_X, bc_right) # ref_s=2 (right)   

Bc1D_setBcType(MyBC, BC_1De_DIRICHLET, 1, AXEe_X )
Bc1D_setBcType(MyBC, BC_1De_DIRICHLET, 2, AXEe_X )

MyRhsFun = Rhs1D_get()
Rhs1D_setFunctionTransientPython(MyRhsFun, 0, AXEe_X, source ) # ref_e=0


MyAdvFun = Adv1D_get()
Adv1D_setFunctionTransientPython(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_X, lambda x,t: 0.0 ) # ref_e=0

 
#############################################################################################

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
 

bCheckGEOMwithBC = Geom1D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X)

#--------------------------------------------------------------------

NBSOMM = GEOM_1D_NBSOMM_get(MyGeom)

MASS   = sp_get(NBSOMM,NBSOMM, 5)
STIFF1 = sp_get(NBSOMM,NBSOMM, 5)
STIFF2 = sp_get(NBSOMM,NBSOMM, 5)

MASS   = assemblage1D_matrix_Mass  ( MyElt , MyGeom , MASS   )
STIFF1 = assemblage1D_matrix_Stiff1( MyElt , MyGeom , STIFF1 )
STIFF2 = assemblage1D_matrix_Stiff2( MyElt , MyGeom , STIFF2 )


A     = sp_get(NBSOMM,NBSOMM, 5)
A_OLD = sp_get(NBSOMM,NBSOMM, 5)

# A = MASSE + kappa.DT.STIFF1 + epsilon.DT.STIFF2

epsilon = Params_get_oneparam(MyParams, "physical_params","epsilon")
kappa   = Params_get_oneparam(MyParams, "physical_params","kappa")


DT      = Params_get_oneparam(MyParams, "time_params", "DT") #  0.0100
TPS_FIN = Params_get_oneparam(MyParams, "time_params", "TPS_FIN") # 50.000000 

NBITER = int( TPS_FIN / DT )
print "NBITER = ",NBITER


sp_zero(A)
A = sp_mltadd(A, MASS  , 1.0        , A )
A = sp_mltadd(A, STIFF1, DT*kappa   , A )
A = sp_mltadd(A, STIFF2, DT*epsilon , A )


sp_compact(A,0.0)

# On sauvegarde A dans A_OLD pour les CL sur le 2nd membre
A_OLD = sp_copy(A) 
#
transform1D_matrix_with_bc( MyElt , MyGeom , MyBC , A )
#
# choleski factorisation of A
LLT = sp_copy(A)
spCHfactor(LLT)
#
# init
U_o = v_get(NBSOMM)
U_o = build_vec_ef_from_function1D( MyElt, MyGeom, MyIC, MyDIC, U_o )
#
U_nm2 = v_get(NBSOMM)
U_nm1 = v_get(NBSOMM)
U_n   = v_get(NBSOMM)
#
U_tps = v_get(NBSOMM)
U_lin = v_get(NBSOMM)
U_nli = v_get(NBSOMM)
#
#
v_copy(U_o, U_nm2)
v_copy(U_o, U_nm1)
#
RHS         = v_get(NBSOMM)
RHS_FUN     = v_get(NBSOMM)
RHS_DISCR   = v_get(NBSOMM)
RHS_UUx     = v_get(NBSOMM)
RHS_LIN     = v_get(NBSOMM)
RHS_LIN1    = v_get(NBSOMM)
RHS_LIN2    = v_get(NBSOMM)
#
#----------------------- graphics interative -------------------------
#
x_left  = Params_get_oneparam(MyParams, "geometry_params","xmin" ) 
x_right = Params_get_oneparam(MyParams, "geometry_params","xmax" )
NBSOMM  = Params_get_oneparam(MyParams, "geometry_params","nx"   )
#
DX = ( x_right - x_left ) / NBSOMM 
#
#
dim_graph = SPMAT_m_get(GEOM_1D_EF_to_WORLD_get(MyGeom))
#
U_graph1 = v_get(dim_graph)
U_graph2 = v_get(NBITER+2)
U_graph3 = v_get(NBITER+2)
#
#
ABSCISSAS1 = Geom1D_getBaseMesh( MyElt ,  MyGeom );
ABSCISSAS2 = v_get(NBITER+2)
for i in range(0,NBITER+2):
   v_set_val(ABSCISSAS2, i, i*DT)
#
#
if  Params_get_oneparam(MyParams, "graphics_output_params", "PGPLOT") or Params_get_oneparam(MyParams, "graphics_output_params", "VOGLE") :
#
    NX = Params_get_oneparam(MyParams, "graphics_interactiv1D_params", "NX")
    NY = Params_get_oneparam(MyParams, "graphics_interactiv1D_params", "NY")
    #
    DRIVER       = Params_get_oneparam(MyParams, "graphics_interactiv1D_params", "DRIVER")
    #
    WINDOW_SIZE_X = Params_get_oneparam(MyParams, "graphics_interactiv1D_params", "SIZE_WINDOW_X")
    WINDOW_SIZE_Y = Params_get_oneparam(MyParams, "graphics_interactiv1D_params", "SIZE_WINDOW_X")
    #
    dim_graph = SPMAT_m_get(GEOM_1D_EF_to_WORLD_get(MyGeom))
    #
    #sp_mv_mlt(MyGeom->EF_to_WORLD, U_o, U_graph1)
    sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), U_o, U_graph1 )
    #
    v_set_val(U_graph2, 0, v_norm2(U_graph1)*sqrt(DX))
    #U_graph2->ve[0] = v_norm2(U_graph1)*sqrt(DX)
    #
    v_set_val(U_graph3, 0, 0.0)
    #U_graph3->ve[0] = 0.0
    #
    #
    #
    if  Params_get_oneparam(MyParams, "graphics_output_params", "PGPLOT") :
        #
        graphics1D_cpgplot_initialize(DRIVER, WINDOW_SIZE_X, WINDOW_SIZE_Y, NX, NY)
        #
        graphics1D_cpgplot_title("Initial Condition")
        #
        graphics1D_cpgplot_labelsize(1,1, Params_get_oneparam(MyParams, "graphics_interactiv1D_params", "SIZE_LABELS"))
        graphics1D_cpgplot_labelsize(1,2, Params_get_oneparam(MyParams, "graphics_interactiv1D_params", "SIZE_LABELS"))
        graphics1D_cpgplot_labelsize(2,1, Params_get_oneparam(MyParams, "graphics_interactiv1D_params", "SIZE_LABELS"))
        graphics1D_cpgplot_labelsize(2,2, Params_get_oneparam(MyParams, "graphics_interactiv1D_params", "SIZE_LABELS"))
        #
        graphics1D_cpgplot_legend(1,1, "Initial Condition")
        graphics1D_cpgplot_legend(1,2, "| U(x,t) |")
        graphics1D_cpgplot_legend(2,1, "| U(x,t) |  (zoom)")
        graphics1D_cpgplot_legend(2,2, "U(x,t) v. d/dtU(x,t)")
        #
        graphics1D_cpgplot_window(1,1,
            Params_get_oneparam2(MyParams, "graphics_interactiv1D_params", "WINDOW_X_MIN", 1,1) ,
            Params_get_oneparam2(MyParams, "graphics_interactiv1D_params", "WINDOW_X_MAX", 1,1) ,
            Params_get_oneparam2(MyParams, "graphics_interactiv1D_params", "WINDOW_Y_MIN", 1,1) ,
            Params_get_oneparam2(MyParams, "graphics_interactiv1D_params", "WINDOW_Y_MAX", 1,1) )
        graphics1D_cpgplot_window(1,2,  0.0, 100.0        ,   0.0,12.0 )
        graphics1D_cpgplot_window(2,1, 20.0,  30.0        ,   5.0,12.0 )
        graphics1D_cpgplot_window(2,2,  6.0,  10.0        ,  -7.0, 7.0 )
        #
        graphics1D_cpgplot_setbox(1,1, 1)
        graphics1D_cpgplot_setbox(1,2, 1)
        graphics1D_cpgplot_setbox(2,1, 1)
        graphics1D_cpgplot_setbox(2,2, 1)
        #
        graphics1D_cpgplot_endmarker(1,1, PGPLOTe_MARKER_UNDEFINED)
        graphics1D_cpgplot_endmarker(1,2, PGPLOTe_MARKER_UNDEFINED)
        graphics1D_cpgplot_endmarker(2,1, PGPLOTe_MARKER_UNDEFINED)
        graphics1D_cpgplot_endmarker(2,2, PGPLOTe_MARKER_MULTIPLICATION)
        #
        graphics1D_cpgplot_curvedata1(1,1, PGPLOTe_COLOR_GREEN , ABSCISSAS1, U_graph1, -1,-1 )
        graphics1D_cpgplot_curvedata1(2,1, PGPLOTe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, 1 )
        graphics1D_cpgplot_curvedata1(1,2, PGPLOTe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, 1 )
        graphics1D_cpgplot_curvedata1(2,2, PGPLOTe_COLOR_RED   , U_graph2  , U_graph3,  0, 1 )
        #
        graphics1D_cpgplot_plotcurves_start()
    #
    #
    if  Params_get_oneparam(MyParams, "graphics_output_params", "VOGLE") :
        #
        graphics1D_vopl_initialize(DRIVER, WINDOW_SIZE_X, WINDOW_SIZE_Y, NX, NY, VOPL_CURVE)
        #
        graphics1D_vopl_title("Initial Condition")
        #
        graphics1D_vopl_legend(1,1, "Initial Condition")
        graphics1D_vopl_legend(1,2, "| U(x,t) |")
        graphics1D_vopl_legend(2,1, "| U(x,t) |  (zoom)")
        graphics1D_vopl_legend(2,2, "U(x,t) v. d/dtU(x,t)")
        #
        graphics1D_vopl_window(1,1, 
            Params_get_oneparam2(MyParams, "graphics_interactiv1D_params", "WINDOW_X_MIN", 1,1) ,
            Params_get_oneparam2(MyParams, "graphics_interactiv1D_params", "WINDOW_X_MAX", 1,1) ,
            Params_get_oneparam2(MyParams, "graphics_interactiv1D_params", "WINDOW_Y_MIN", 1,1) ,
            Params_get_oneparam2(MyParams, "graphics_interactiv1D_params", "WINDOW_Y_MAX", 1,1) )
        graphics1D_vopl_window(1,2,  0.0, 100.0        ,   0.0,12.0 )
        graphics1D_vopl_window(2,1, 20.0,  30.0        ,   5.0,12.0 )
        graphics1D_vopl_window(2,2,  6.0,  10.0        ,  -7.0, 7.0 )
        #
        graphics1D_vopl_setbox(1,1, 1)
        graphics1D_vopl_setbox(1,2, 1)
        graphics1D_vopl_setbox(2,1, 1)
        graphics1D_vopl_setbox(2,2, 1)
        #
        graphics1D_vopl_endmarker(1,1, VOGLEe_NO_MARKER)
        graphics1D_vopl_endmarker(1,2, VOGLEe_NO_MARKER)
        graphics1D_vopl_endmarker(2,1, VOGLEe_NO_MARKER)
        graphics1D_vopl_endmarker(2,2, VOGLEe_MARKER_CHRISMAS)
        #
        graphics1D_vopl_curvedata1(1,1, PGPLOTe_COLOR_GREEN , ABSCISSAS1, U_graph1, -1,-1 )
        graphics1D_vopl_curvedata1(2,1, PGPLOTe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, 1 )
        graphics1D_vopl_curvedata1(1,2, PGPLOTe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, 1 )
        graphics1D_vopl_curvedata1(2,2, PGPLOTe_COLOR_RED   , U_graph2  , U_graph3,  0, 1 )
        #
        graphics1D_vopl_plotcurves_start()
#
#
#---------------------------------------------------------------------
#
for n in range(1,2):
    #
    #
    # set the cuurent time in the functions structs
    Bc1D_setTps ( MyBC    , n*DT )
    Rhs1D_setTps( MyRhsFun, n*DT )
    #
    #
    # term from time discretisation
    RHS_DISCR= sp_mv_mlt( MASS, U_nm1, RHS_DISCR)                                #  RHS_DISCR = MASS * U_nm1
    #
    # term from rhs function
    RHS_FUN = assemblage1D_vector_fun( MyElt, MyGeom, MyRhsFun , RHS_FUN )       #  RHS_FUN   = DT * F_n
    #
    # term from convection
    RHS_UUx = assemblage1D_vector_abx( MyElt, MyGeom, U_nm1, U_nm1 , RHS_UUx )   #  RHS_UUx   = DT * [ U_nm1 * GRAD( U_nm1 ) ]
    #
    #
    v_zero(RHS)
    v_mltadd( RHS , RHS_DISCR, 1.0 , RHS )
    v_mltadd( RHS , RHS_UUx  , -DT , RHS )
    v_mltadd( RHS , RHS_FUN  ,  DT , RHS )
    #
    #
    # finally, set the bc on the rhs
    transform1D_vector_with_bc( MyElt , MyGeom , MyBC , A_OLD , RHS )
    #
    # solve the system 
    U_n = spCHsolve( LLT, RHS, U_n )	
    #
    #
    #-----------------------  statistics -------------------------
    #
    itstat = Params_get_oneparam(MyParams, "miscellaneous_params", "itstat")
    #
    if ( (itstat > 0) and (n % itstat) == 0 ):

        diff = 0.0
       
        diff = diff_vec_function1Dtransient( MyElt, MyGeom, U_nm1, fun1Dex, 0.0, RHS )
        print "\ntime = 0.0  --> diff solexacte-solapprochee = ", diff*sqrt(DX)

        diff = diff_vec_function1Dtransient( MyElt, MyGeom, U_n, fun1Dex, n*DT, RHS )
        print "time = " , n*DT , " --> diff solexacte-solapprochee = " , diff*sqrt(DX)
    #
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

            title = "SOL_PGPLOT_%010.5lf" %(n*DT)
            graphics1D_cpgplot_title(title)

            legend = "U(x,t)  ( t = %010.5lf )" %(n*DT)
            graphics1D_cpgplot_legend(1,1, legend)

            graphics1D_cpgplot_curvedata1(1,1, PGPLOTe_COLOR_GREEN , ABSCISSAS1, U_graph1, -1,-1 )
            graphics1D_cpgplot_curvedata1(2,1, PGPLOTe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, n )
            graphics1D_cpgplot_curvedata1(1,2, PGPLOTe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, n )
            graphics1D_cpgplot_curvedata1(2,2, PGPLOTe_COLOR_RED   , U_graph2  , U_graph3,  0, n )

            graphics1D_cpgplot_plotcurves_flush()

        # -- Graphics with VOGLE library ---- 
        if Params_get_oneparam(MyParams, "graphics_output_params", "VOGLE") :

            title = "SOL_VOGLE_%010.5lf" %(n*DT)
            graphics1D_vopl_title(title)

            legend = "U(x,t)  ( t = %010.5lf )" %(n*DT)
            graphics1D_vopl_legend(1,1, legend)

            graphics1D_vopl_curvedata1(1,1, VOGLEe_COLOR_GREEN , ABSCISSAS1, U_graph1, -1,-1 )
            graphics1D_vopl_curvedata1(2,1, VOGLEe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, n )
            graphics1D_vopl_curvedata1(1,2, VOGLEe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, n )
            graphics1D_vopl_curvedata1(2,2, VOGLEe_COLOR_BLUE  , U_graph2  , U_graph3,  0, n )

            graphics1D_vopl_plotcurves_flush()

        
    #--------------------------------------------------------------------
    #
    v_copy( U_nm1, U_nm2 )
    v_copy( U_n  , U_nm1 )
    #   
#
#
#
sp_zero(A)
A = sp_mltadd(A, MASS  , 1.5            , A )
A = sp_mltadd(A, STIFF1, 0.5*DT*kappa   , A )
A = sp_mltadd(A, STIFF2, 0.5*DT*epsilon , A )
#
# 
sp_compact(A,0.0)
#
# On sauvegarde A dans A_OLD pour les CL sur le 2nd membre
A_OLD = sp_copy(A) 
#
transform1D_matrix_with_bc( MyElt , MyGeom , MyBC , A )
#
# choleski factorisation of A
sp_copy2(A, LLT)
spCHfactor(LLT)
#
#
for n in range(2,NBITER+1):
#
    #
    # set the cuurent time in the functions structs
    Bc1D_setTps ( MyBC    , n*DT )
    Rhs1D_setTps( MyRhsFun, n*DT )
    #
    #
    v_zero( U_tps )
    U_tps = v_mltadd( U_tps , U_nm1 ,  2.0 , U_tps )
    U_tps = v_mltadd( U_tps , U_nm2 , -0.5 , U_tps )
    #
    v_zero( U_lin )
    U_lin = v_mltadd( U_lin , U_nm1 ,  0.5 , U_lin )
    U_lin = v_mltadd( U_lin , U_nm2 ,  0.0 , U_lin )
    #
    v_zero( U_nli )
    U_nli = v_mltadd( U_nli , U_nm1 ,  2.0 , U_nli )
    U_nli = v_mltadd( U_nli , U_nm2 , -1.0 , U_nli ) 
    #
    #
    # term from time discretisation
    RHS_DISCR= sp_mv_mlt( MASS, U_tps, RHS_DISCR)                                  #  RHS_DISCR = MASS * U_tps
    #
    # term from rhs function
    RHS_FUN = assemblage1D_vector_fun( MyElt, MyGeom, MyRhsFun , RHS_FUN )         #  RHS_FUN   = DT * F_n
    #
    # term from convection
    RHS_UUx = assemblage1D_vector_abx( MyElt, MyGeom, U_nli, U_nli , RHS_UUx )     #  RHS_UUx   = DT * [ U_nli * GRAD( U_nli ) ]
    #
    # term from linearity
    RHS_LIN1 = sp_mv_mlt( STIFF1 , U_lin , RHS_LIN1 )
    sv_mlt( kappa , RHS_LIN1 , RHS_LIN1 )
    RHS_LIN2 = sp_mv_mlt( STIFF2 , U_lin , RHS_LIN2 )
    sv_mlt( epsilon , RHS_LIN2 , RHS_LIN2 )
    #
    RHS_LIN = v_add( RHS_LIN1 , RHS_LIN2 , RHS_LIN )
    #
    #
    #
    v_zero(RHS)
    v_mltadd( RHS , RHS_DISCR, 1.0 , RHS )
    v_mltadd( RHS , RHS_UUx  , -DT , RHS )
    v_mltadd( RHS , RHS_FUN  ,  DT , RHS )
    v_mltadd( RHS , RHS_LIN  , -DT , RHS )
    #
    #
    # finally, set the bc on the rhs
    transform1D_vector_with_bc( MyElt , MyGeom , MyBC , A_OLD , RHS )
    #
    # solve the system 
    U_n = spCHsolve( LLT, RHS, U_n )	
    #
    #
    #-----------------------  statistics -------------------------
    #
    itstat = Params_get_oneparam(MyParams, "miscellaneous_params", "itstat")
    #
    if (  (itstat > 0) and (n % itstat) == 0 ):

        diff = 0
        diff = diff_vec_function1Dtransient( MyElt, MyGeom, U_n, fun1Dex, n*DT, RHS )
        print "time = " , n*DT , " --> diff solexacte-solapprochee = " , diff*sqrt(DX)
    
	 
    #----------------------- graphics interativ -------------------------
    #
    itgraph = Params_get_oneparam(MyParams, "miscellaneous_params", "itgraph")
    
    #U_graph1 = sp_mv_mlt(MyGeom->EF_to_WORLD, U_n, U_graph1)
    sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), U_n, U_graph1 )
    #U_graph2->ve[k] = v_norm2(U_graph1)*sqrt(DX)
    v_set_val(U_graph2, n, v_norm2(U_graph1)*sqrt(DX) )
    #U_graph3->ve[k] = ( U_graph2->ve[n] - U_graph2->ve[n-1] ) / DT
    v_set_val(U_graph3, n, ( v_get_val(U_graph2,n) - v_get_val(U_graph2,n-1) ) / DT )
		  
    if (n % itgraph) == 0 :
                
        # -- Graphics with PGPLOT library ----
        if Params_get_oneparam(MyParams, "graphics_output_params", "PGPLOT") :

            title = "SOL_PGPLOT_%010.5lf" %(n*DT)
            graphics1D_cpgplot_title(title)
				
            legend = "U(x,t)  ( t = %010.5lf )" %(n*DT)
            graphics1D_cpgplot_legend(1,1, legend)

            graphics1D_cpgplot_curvedata1(1,1, PGPLOTe_COLOR_GREEN , ABSCISSAS1, U_graph1, -1,-1 )
            graphics1D_cpgplot_curvedata1(2,1, PGPLOTe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, n )
            graphics1D_cpgplot_curvedata1(1,2, PGPLOTe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, n )
            graphics1D_cpgplot_curvedata1(2,2, PGPLOTe_COLOR_RED   , U_graph2  , U_graph3,  0, n )

            graphics1D_cpgplot_plotcurves_flush()

        # -- Graphics with VOGLE library ----
        if Params_get_oneparam(MyParams, "graphics_output_params", "VOGLE") :

            title = "SOL_VOGLE_%010.5lf" %(n*DT)
            graphics1D_vopl_title(title)

            legend = "U(x,t)  ( t = %010.5lf )" %(n*DT)
            graphics1D_vopl_legend(1,1, legend)

            graphics1D_vopl_curvedata1(1,1, VOGLEe_COLOR_GREEN , ABSCISSAS1, U_graph1, -1,-1 )
            graphics1D_vopl_curvedata1(2,1, VOGLEe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, n )
            graphics1D_vopl_curvedata1(1,2, VOGLEe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, n )
            graphics1D_vopl_curvedata1(2,2, VOGLEe_COLOR_RED   , U_graph2  , U_graph3,  0, n )

            graphics1D_vopl_plotcurves_flush()

    #--------------------------------------------------------------------
    #
    v_copy( U_nm1, U_nm2 )
    v_copy( U_n  , U_nm1 )
    #
#   
#
#
#
if Params_get_oneparam(MyParams, "graphics_output_params", "PGPLOT") :
    graphics1D_cpgplot_finalize_transient()
#
if Params_get_oneparam(MyParams, "graphics_output_params", "VOGLE") :
    graphics1D_vopl_finalize_transient()
#
#
# free memory
# -----------
#
ELT_1D_FREE(MyElt)
#
GEOM_1D_FREE(MyGeom)
BC_1D_FREE(MyBC)
RHS_1D_FREE(MyRhsFun)
ADV_1D_FREE(MyAdvFun)
FUN_1D_FREE(MyIC)
#
PARAMS_FREE(MyParams)
#
#---------------------------------------------------------------------
#
#mem_info_file(stdout,0)
#   
#mem_info_file(stdout,MY_LIST1)
#mem_info_file(stdout,MY_LIST2) 
#mem_info_file(stdout,MY_LIST3) 
#mem_info_file(stdout,MY_LIST4) 
#mem_info_file(stdout,MY_LIST5) 
#mem_info_file(stdout,MY_LIST6) 
#mem_info_file(stdout,MY_LIST7)
# 
fp2.close()
#
#----------------------------------------------------------------------
