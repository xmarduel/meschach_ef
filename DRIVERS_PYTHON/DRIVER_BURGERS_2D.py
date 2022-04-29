#! /usr/bin/env python

from sys import *
from math import *

from meschach import *
from meschach_adds import *
from meschach_ef import *
from meschach_cpgplot import *

#from PDE_ALGO_BURGERS_2D import *
#from PDE_ALGO_NAVIERSTOKES_2D import *   # ToDo

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
set_warn_flag(EF_SILENT)

#-------------------------------------------------------------------

def spheric_src(x,y):
    return ( - spheric_xx(x,y) - spheric_yy(x,y) + spheric(x,y)*spheric_x(x,y) + spheric(x,y)*spheric_y(x,y) + spheric(x,y)*spheric(x,y))
    #return ( - spheric_xx(x,y) - spheric_yy(x,y) + spheric(x,y)*spheric(x,y) )
#
def spheric(x,y):
    #return 10.0/4.0*(0.5 - (x-0.5)*(x-0.5) - (y-0.5)*(y-0.5))
    return 10.0/4.0*( 0.5 - (x-0.5)*(x-0.5)*x - (y-0.5)*(y-0.5)*y )
def spheric_x(x,y):
    #return 10.0/4.0*( - 2*(x-0.5) )
    return 10.0/4.0*( - 2*(x-0.5)*x - (x-0.5)*(x-0.5) )
def spheric_y(x,y):
    #return 10.0/4.0*( - 2*(y-0.5) )
    return 10.0/4.0*( - 2*(y-0.5)*y - (y-0.5)*(y-0.5) )
def spheric_xx(x,y):
    #return 10.0/4.0*( - 2.0 )
    return 10.0/4.0*( - 2*(x-0.5) -2*x  - 2*(x-0.5) )
def spheric_yy(x,y):
    #return 10.0/4.0*( - 2.0 )
    return 10.0/4.0*( - 2*(y-0.5) -2*y  - 2*(y-0.5) )

def bc0(x,y):
    return 0.0



MyBC = Bc2D_get()

Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_X)
Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_X)
Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 3, AXEe_X)
Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 4, AXEe_X)

Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_X, spheric)
Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_X, spheric)
Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_X, spheric)
Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 4, AXEe_X, spheric)

Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 1, AXEe_X, bc0)
Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 2, AXEe_X, bc0)
Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 3, AXEe_X, bc0)
Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 4, AXEe_X, bc0)


MyRhsFun = Rhs2D_get()
Rhs2D_setFunctionPython(MyRhsFun, 0, AXEe_X, spheric_src) # ref_e=0
Rhs2D_setFunctionPython(MyRhsFun, 0, AXEe_Y, spheric_src) # ref_e=0


MyAdv = Adv2D_get()
MyAdv = Adv2D_setFunctionPython( MyAdv, 0, AXEe_X, AXEe_X, AXEe_X, spheric)
MyAdv = Adv2D_setFunctionPython( MyAdv, 0, AXEe_X, AXEe_X, AXEe_Y, spheric)
MyAdv = Adv2D_setFunctionPython( MyAdv, 0, AXEe_X, AXEe_Y, AXEe_X, spheric)
MyAdv = Adv2D_setFunctionPython( MyAdv, 0, AXEe_Y, AXEe_Y, AXEe_Y, spheric)


a = Adv2D_evalFunction(MyAdv, 0, AXEe_X, AXEe_X, AXEe_X, 1, 1)
print "a=", a
a = Adv2D_evalFunction(MyAdv, 0, AXEe_X, AXEe_X, AXEe_Y, 1, 1)
print "a=", a
a = Adv2D_evalFunction(MyAdv, 0, AXEe_X, AXEe_Y, AXEe_X, 1, 1)
print "a=", a
a = Adv2D_evalFunction(MyAdv, 0, AXEe_Y, AXEe_Y, AXEe_Y, 1, 1)
print "a=", a
a = Adv2D_evalFunction(MyAdv, 0, AXEe_Y, AXEe_Y, AXEe_Z, 1, 1)
print "a=", a

#-------------------------------------------------------------------   

# LECTURE  IN  INPUT FILE "INPUT_PDE.dat"
# ECRITURE IN OUTOUT FILE "OUTPUT_PDE.dat"

fp2 = open("OUTPUT.txt", "w")

MyParams = Params_get()
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "input_output_files","fp2", fp2)
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "main_problem","NULL", "Burgers" )  # Laplacian
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "finite_elements_params","name_ef", "P3" )    # Type d'EF : "P1","P2", "P3"

Params_set_oneparam(MyParams, "matrix_solver_params","resolution_method", "DIRECT-METHOD" )     # Methode : DIRECT-METHOD,CG,CGS,GMRES(k) 
Params_set_oneparam(MyParams, "matrix_solver_params","preconditionning", "NULL" )              # Precond : NULL, ICH, ILU, DIAG. 

Params_set_oneparam(MyParams, "geometry_params","meshfile", "MESH_P1.quad" )  #  Mesh File ("name.dat") "MESH_P1.quad" "cube2D_P1_v3_ok2.gmsh"
Params_set_oneparam(MyParams, "geometry_params","meshtype", "quad1" )  #  Mesh File ("name.dat") "MESH_P1.quad2" "cube2D_P1_v3_ok2.gmsh"
Params_set_oneparam(MyParams, "geometry_params","meshname", "MESH_20" )  #  Mesh Name inside meshfile

Params_set_oneparam(MyParams, "graphics_output_params","GNUPLOT",     1) # GNUPLOT
Params_set_oneparam(MyParams, "graphics_output_params","GRAPH",       0) # GRAPH
Params_set_oneparam(MyParams, "graphics_output_params","MATLAB",      0) # MATLAB
Params_set_oneparam(MyParams, "graphics_output_params","VTK",         1) # VTK
Params_set_oneparam(MyParams, "graphics_output_params","SILO",        0) # SILO
Params_set_oneparam(MyParams, "graphics_output_params","PGPLOT",      0) # PGPLOT
Params_set_oneparam(MyParams, "graphics_output_params","VOGLE",       0) # VOGLE
Params_set_oneparam(MyParams, "graphics_output_params","LIBSCIPLOT",  0) # LIBSCIPLOT   (0=NO;1=YES)

Params_set_oneparam(MyParams, "miscellaneous_graphics","ispmatpattern", 1 ) # IHBF
Params_set_oneparam(MyParams, "miscellaneous_graphics","igeomview",    1 ) # IHBF


#-physical parameters------------------------------------------------------------------

Params_set_oneparam(MyParams, "physical_params","epsilon",    0.0000) # epsilon
Params_set_oneparam(MyParams, "physical_params","sigma",      1.0   ) # sigma
Params_set_oneparam(MyParams, "physical_params","kappa",      1.0   ) # kappa  -k*LAPLACIEN(u)+ e.BILAPL(u) +sigma.u =F

Params_set_oneparam(MyParams, "convective_terms_params","delta",     0.10) # delta
Params_set_oneparam(MyParams, "convective_terms_params","rho_stab",  1.50) # rho_stab parametres of stab (SUPG,GALS ..)

#-parameter for the resolution of the linear system------------------------------------

Params_set_oneparam(MyParams, "matrix_solver_params","max_steps",  500  ) # max_iter Nombre max d'iter de la methode iterative
Params_set_oneparam(MyParams, "matrix_solver_params","eps_steps", 1.e-12) # eps_iter arret des iterations "  "  "   "

Params_set_oneparam(MyParams, "miscellaneous_params","iter_info", ITER_INFOe_ALL )       # iter_info (0 , 1=stdout, 2=file iter_file)
Params_set_oneparam(MyParams, "miscellaneous_params","iter_file", "the_residuals.dat"  ) # iter_file le nom du fichier

Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_SPOOLES)       # USE BAND WIDTH REDUCTION


Params_set_staticparam(MyParams, 0)


MyElt  = elt2D_get(Params_get_oneparam(MyParams, "finite_elements_params", "name_ef"))

elt_P3 = elt2D_get("P3")
elt_P2 = elt2D_get("P2")
elt_P1 = elt2D_get("P1")

#--------------------------------------------------------------------------------------------

MyGeom = Geom2D_get(MyElt, 
        Params_get_oneparam(MyParams, "geometry_params", "meshfile"),
        Params_get_oneparam(MyParams, "geometry_params", "meshname"),
        Params_get_oneparam(MyParams, "geometry_params", "meshtype"))

Geom2D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X)   

# -------------------------------------------------------------------------------------------

PROBLEM = Params_get_oneparam(MyParams, "main_problem", "NULL")
print "PROBLEM is ",PROBLEM


if   PROBLEM == "LAPLACIAN" :

    SOL = solve2D_laplacien( MyElt , MyGeom , MyBC , MyRhsFun )    

elif PROBLEM == "CONVECTION-DIFFUSION" :

    SOL = solve2D_convection_diffusion( MyElt , MyGeom , MyBC , MyRhsFun )

elif PROBLEM == "BURGERS" :

    #SOL = Py_solve2D_burgers1( MyElt , MyGeom , MyBC , MyRhsFun , MyParams)
    SOL = Py_solve2D_burgers2( MyElt , MyGeom , MyBC , MyRhsFun , MyParams)

elif PROBLEM == "STOKES" :

    (U,V,P) = Py_solve2D_stokes( MyElt , elt_P1 , MyGeom , MyBC , MyRhsFun )

elif PROBLEM == "NAVIER-STOKES" :

    (U,V,P) = Py_solve2D_navier_stokes( MyElt , elt_P1 , MyGeom , MyBC , MyRhsFun , MyParams)

else:

    print "Problem \"%s\" not yet implemented" % PROBLEM
    exit(EXIT_FAILURE)

#----------------------- graphics output -----------------------------

if   PROBLEM == "LAPLACIAN" :

    graphics2D( "gnuplot" , MyElt , MyGeom , SOL , "SolApproch")
    graphics2D( "vtk"     , MyElt , MyGeom , SOL , "SolApproch")

elif PROBLEM == "CONVECTION-DIFFUSION" :

    graphics2D( "gnuplot" , MyElt , MyGeom , SOL , "SolApproch")
    graphics2D( "vtk"     , MyElt , MyGeom , SOL , "SolApproch")

elif PROBLEM == "BURGERS" :

    graphics2D( "gnuplot" , MyElt , MyGeom , SOL , "SolApproch")
    graphics2D( "vtk"     , MyElt , MyGeom , SOL , "SolApproch")

elif PROBLEM == "STOKES" :

    #graphics2D( "vtk" , MyElt , MyGeom , U , "Sol_U_Approch")
    #graphics2D( "vtk" , MyElt , MyGeom , V , "Sol_V_Approch")
    #graphics2D( "vtk" , MyElt , MyGeom , P , "Sol_P_Approch")
    #
    graphics2D_stokes( "vtk", MyElt , MyGeom , U, V, P, "Stokes")

elif PROBLEM == "NAVIER-STOKES" :

    pass

else:

    print "Problem \"%s\" not yet implemented" % PROBLEM
    exit(EXIT_FAILURE)

#---------------------------------------------------------------------

if True:

    fun2D = Fun2D_get()
    Fun2D_setFunctionPython(fun2D, spheric) # ref_e=0, axe=1
    #Fun2D_setCFunction(fun2D, ExSol3D[MyParams->misc_params.Exact_sol] ) 

    diff = diff_vec_function2D( MyElt, MyGeom, SOL, fun2D, SOL )
    print "\ndiff solexacte-solapprochee = %le" % diff
    
    FUN_2D_FREE(fun2D)



# free memory
# -----------

ELT_2D_FREE(MyElt)

ELT_2D_FREE(elt_P1)
ELT_2D_FREE(elt_P2)
ELT_2D_FREE(elt_P3)

GEOM_2D_FREE(MyGeom)
BC_2D_FREE(MyBC)
RHS_2D_FREE(MyRhsFun)

#V_FREE(U)
#V_FREE(V)
#V_FREE(P)

PARAMS_FREE(MyParams)

#---------------------------------------------------------------------

#mem_info_file(stdout,0)

#mem_info_file(stdout,MY_LIST1)
#mem_info_file(stdout,MY_LIST2) 
#mem_info_file(stdout,MY_LIST3) 
#mem_info_file(stdout,MY_LIST4) 
#mem_info_file(stdout,MY_LIST5) 
#mem_info_file(stdout,MY_LIST6)
#mem_info_file(stdout,MY_LIST7) 

fp2.close()

#----------------------------------------------------------------------
