#! /usr/bin/env python
#
#
from sys import *
from math import *
#
from meschach import *
from meschach_adds import *
from meschach_ef import *
from meschach_cpgplot import *
from meschach_spooles import *
#
#-------------------------------------------------------------------
#  
mem_attach_list1()
mem_attach_list2()
mem_attach_list3()
mem_attach_list4()
mem_attach_list5()
mem_attach_list6()
mem_attach_list7()
#
err_attach_list2()
err_attach_list3()
err_attach_list4()
err_attach_list5()
err_attach_list6()
err_attach_list7()
#
set_err_flag(EF_JUMP)
set_warn_flag(EF_SILENT)
#
#-------------------------------------------------------------------
#
#
def source(x,y,z):
    return  0.0
#
def bc0(x,y,z):
    return (0.0)
def bc1(x,y,z):
    return (1.0)
def bc2(x,y,z):
    return (0.0)
def bc2(x,y,z):
    return (0.0)
def bc3(x,y,z):
    return (0.0)
#
#
def cv1(x,y,z):
    #print "cv1: x,y,z=",x,y,z 
    return  ( 20*y*(1-y)*x*(1-x) )
def cv2(x,y,z):
    #print "cv2: x,y,z=",x,y,z
    return  ( 20*y*(1-y)*z*(1-z) )
def cv3(x,y,z):
    print "cv3: x,y,z=",x,y,z 
    return  ( 10*x*(1-x)*z*(1-z) )
#
#
#
MyBC = Bc3D_get()
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 1, AXEe_X )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 2, AXEe_X )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 3, AXEe_X )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 4, AXEe_X )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 5, AXEe_X )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 6, AXEe_X )
#
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 1, AXEe_Y )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 2, AXEe_Y )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 3, AXEe_Y )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 4, AXEe_Y )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 5, AXEe_Y )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 6, AXEe_Y )
#
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 1, AXEe_Z )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 2, AXEe_Z )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 3, AXEe_Z )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 4, AXEe_Z )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 5, AXEe_Z )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 6, AXEe_Z )
#
#
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 1, AXEe_X, cv1) # z = 1
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 2, AXEe_X, bc0) # x = 1
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 3, AXEe_X, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 4, AXEe_X, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 5, AXEe_X, bc0) # z = 0
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 6, AXEe_X, bc0)
#
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 1, AXEe_Y, bc0) 
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 2, AXEe_Y, bc0) # x = 1
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 3, AXEe_Y, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 4, AXEe_Y, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 5, AXEe_Y, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 6, AXEe_Y, bc0)
#
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 1, AXEe_Z, bc0) 
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 2, AXEe_Z, cv2)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 3, AXEe_Z, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 4, AXEe_Z, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 5, AXEe_Z, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 6, AXEe_Z, bc0)
#
#
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 1, AXEe_X, bc0) # z = 1
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 2, AXEe_X, bc0) # x = 1
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 3, AXEe_X, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 4, AXEe_X, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 5, AXEe_X, bc0) # z = 0
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 6, AXEe_X, bc0)
#
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 1, AXEe_Y, bc0) 
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 2, AXEe_Y, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 3, AXEe_Y, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 4, AXEe_Y, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 5, AXEe_Y, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 6, AXEe_Y, bc0)
#
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 1, AXEe_Z, bc0) 
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 2, AXEe_Z, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 3, AXEe_Z, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 4, AXEe_Z, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 5, AXEe_Z, bc0)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 6, AXEe_Z, bc0)
#
#

#
MyRhsFun = Rhs3D_get()
#Rhs3D_setFunctionPython(MyRhsFun, 0, AXEe_X, source) # ref_e=0
#Rhs3D_setFunctionPython(MyRhsFun, 0, AXEe_Y, source) # ref_e=0
#Rhs3D_setFunctionPython(MyRhsFun, 0, AXEe_Z, source) # ref_e=0
Rhs3D_setCFunction(MyRhsFun, 0, AXEe_X, Src3D_00) # ref_e=0
Rhs3D_setCFunction(MyRhsFun, 0, AXEe_Y, Src3D_00) # ref_e=0
Rhs3D_setCFunction(MyRhsFun, 0, AXEe_Z, Src3D_00) # ref_e=0
#
#
#-------------------------------------------------------------------   

# LECTURE  IN  INPUT FILE "INPUT_PDE.dat"
# ECRITURE IN OUTOUT FILE "OUTPUT_PDE.dat"
      
fp2 = open("OUTPUT.txt", "w")

MyParams = Params_get()

#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "input_output_files","fp2", fp2)
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "main_problem","NULL", "Stokes" )
#
Params_set_oneparam(MyParams, "stokes_params","method", "PRESSUREMATRIX" ) # "PRESSUREMATRIX" "UZAWA"
Params_set_oneparam(MyParams, "stokes_params", "pressurematrix.iterative_solver.bandwidth_option", "BANDWRe_SPOOLES" )  
Params_set_oneparam(MyParams, "stokes_params", "pressurematrix.iterative_solver.bandwidth_method", "BANDWRe_SPOOLES_NONSYM" ) 
Params_set_oneparam(MyParams, "stokes_params", "pressurematrix.innerloop_method", "DIRECT-METHOD" )  # "DIRECT-METHOD" "ITERATIV-METHOD"  
#Params_set_oneparam(MyParams, "stokes_params", "pressurematrix.iterative_solver.eps_steps", 1.0e-12 )  
#Params_set_oneparam(MyParams, "stokes_params", "pressurematrix.iterative_solver.max_steps", 500 )

Params_set_oneparam(MyParams, "stokes_params", "uzawa_rho"         , 0.50 ) #  for "UZAWA" 
Params_set_oneparam(MyParams, "stokes_params", "augmentedlagr_rho" , 0.50 ) #  for "AUGMENTEDLAGR"
#
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "finite_elements_params","name_ef", "P2" )    # Type d'EF : "P1","P2", "P1b"
#
Params_set_oneparam(MyParams, "geometry_params","meshfile", "cube3D_P1.cube" )  #  Mesh File ("name.dat") 
#Params_set_oneparam(MyParams, "geometry_params","meshfile", "cube3D_P1_v6.gmsh" )  #  Mesh File ("name.dat") 
Params_set_oneparam(MyParams, "geometry_params","meshtype", "cube" )     # 
Params_set_oneparam(MyParams, "geometry_params","meshname", "MESH_10" )     # only for "cube" files
#
Params_set_oneparam(MyParams, "graphics_output_params","GNUPLOT",     0) # GNUPLOT
Params_set_oneparam(MyParams, "graphics_output_params","GRAPH",       0) # GRAPH
Params_set_oneparam(MyParams, "graphics_output_params","MATLAB",      0) # MATLAB
Params_set_oneparam(MyParams, "graphics_output_params","VTK",         1) # VTK
Params_set_oneparam(MyParams, "graphics_output_params","SILO",        1) # SILO
Params_set_oneparam(MyParams, "graphics_output_params","PGPLOT",      0) # PGPLOT
Params_set_oneparam(MyParams, "graphics_output_params","VOGLE",       0) # VOGLE
Params_set_oneparam(MyParams, "graphics_output_params","LIBSCIPLOT",  0) # LIBSCIPLOT   (0=NO;1=YES)
#
#
#-physical parameters------------------------------------------------------------------
#
Params_set_oneparam(MyParams, "physical_params","epsilon",    0.0  ) # epsilon
Params_set_oneparam(MyParams, "physical_params","sigma",      0.0   ) # sigma
Params_set_oneparam(MyParams, "physical_params","kappa",      1.0   ) # kappa  -k*LAPLACIEN(u)+ e.BILAPL(u) +sigma.u =F
#
#Params_set_oneparam(MyParams, "convective_terms_params","b1",   1   )
#Params_set_oneparam(MyParams, "convective_terms_params","b2",   1   )
#Params_set_oneparam(MyParams, "convective_terms_params","b3",   1   ) # choix de b=(b1,b2,b3) in -e*DD(u) + b.D(u) +sigma.u =F
#
Params_set_oneparam(MyParams, "convective_terms_params","delta",     0.10) # delta
Params_set_oneparam(MyParams, "convective_terms_params","rho_stab",  0.50) # rho_stab parametres of stab (SUPG,GALS ..)
#
#-parameter for the resolution of the linear system------------------------------------
#
Params_set_oneparam(MyParams, "matrix_solver_params","max_steps",   500  ) # max_iter de la methode iterative
Params_set_oneparam(MyParams, "matrix_solver_params","eps_steps", 1.e-8) # eps_iter arret des iterations "  "  "   "
#Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_MESCHACH)       # BANDWRe_MESCHACH BANDWRe_SPOOLES
#Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_AMD)       # BANDWRe_MESCHACH BANDWRe_SPOOLES
#Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_SPOOLES)       # BANDWRe_MESCHACH BANDWRe_SPOOLES
#Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_BAND)       # USE BAND WIDTH REDUCTION
#Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_PROFILE)       # USE BAND WIDTH REDUCTION
#Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_SPOOLES_SYM)   # USE BAND WIDTH REDUCTION
#Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_SPOOLES_NONSYM)   # USE BAND WIDTH REDUCTION
#Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_MESCHACH_BAND)       # USE BAND WIDTH REDUCTION
#Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_MESCHACH_PROFILE)       # USE BAND WIDTH REDUCTION


Params_set_oneparam(MyParams, "miscellaneous_params","iter_info", ITER_INFOe_ALL   )     # iter_info 
Params_set_oneparam(MyParams, "miscellaneous_params","iter_file", "the_residuals.dat"  ) # iter_file le nom du fichier


Params_set_staticparam(MyParams, 0)


MyElt  = elt3D_get(Params_get_oneparam(MyParams, "finite_elements_params", "name_ef"))

elt_P2 = elt3D_get("P2")
elt_P1 = elt3D_get("P1")

if   ( "P2" == ELT_3D_name_ef_get(MyElt) ) :
    MyEltM1 = elt_P1
elif ( "P1b"== ELT_3D_name_ef_get(MyElt) ) :
    MyEltM1 = elt_P1
elif ( "P1" == ELT_3D_name_ef_get(MyElt) ) :
    MyEltM1 = elt_P1
else :
    pass

#--------------------------------------------------------------------

MyGeom = Geom3D_get(MyElt, Params_get_oneparam(MyParams, "geometry_params", "meshfile"))

Geom3D_check_with_boundaryconditions(MyGeom , MyBC, AXEe_X )  
Geom3D_check_with_boundaryconditions(MyGeom , MyBC, AXEe_Y )
Geom3D_check_with_boundaryconditions(MyGeom , MyBC, AXEe_Z )

#--------------------------------------------------------------------

PROBLEM = Params_get_oneparam(MyParams, "main_problem", "NULL")
print "PROBLEM is ", PROBLEM
METHOD = Params_get_oneparam(MyParams, "stokes_params", "method")
print "METHOD is ", METHOD


if   PROBLEM == "LAPLACIAN" :

    SOL = solve3D_laplacien( MyElt , MyGeom , MyBC , MyRhsFun )   

elif PROBLEM == "CONVECTION-DIFFUSION" :

    SOL = solve3D_convection_diffusion( MyElt , MyGeom , MyBC , MyRhsFun )

elif PROBLEM == "STOKES" :

    (U,V,W,P) = Py_solve3D_stokes( MyElt , MyEltM1 , MyGeom , MyBC , MyRhsFun )
	
elif PROBLEM == "NAVIER-STOKES" :

    (U,V,W,P) = Py_solve3D_navier_stokes( MyElt , MyEltM1 , MyGeom , MyBC , MyRhsFun )

else:

	 print "Problem \"%s\" not yet implemented" % PROBLEM
    exit(EXIT_FAILURE)

   
#----------------------- graphics output -----------------------------

if    PROBLEM == "LAPLACIAN"  :
	
    graphics3D( "vtk" , MyElt , MyGeom , SOL , "Sol_Approch3D")

elif PROBLEM == "CONVECTION-DIFFUSION" :

    graphics3D( "vtk" , MyElt , MyGeom , SOL , "Sol_Approch3D")

elif PROBLEM == "STOKES" :

    graphics3D( "vtk" , MyElt , MyGeom , U , "Stokes3D_U_")
    graphics3D( "vtk" , MyElt , MyGeom , V , "Stokes3D_V_")
    graphics3D( "vtk" , MyElt , MyGeom , W , "Stokes3D_W_")
    graphics3D( "vtk" , MyEltM1 , GEOM_3D_geomBase_get(MyGeom), P , "Stokes3D_P_")
    graphics3D_stokes("vtk", MyElt , MyGeom , U,V,W,P, "Stokes3D")
        
    #graphics3D( "silo" , MyElt , MyGeom , U , "Stokes3D_U_")
    #graphics3D( "silo" , MyElt , MyGeom , V , "Stokes3D_V_")
    #graphics3D( "silo" , MyElt , MyGeom , W , "Stokes3D_W_")
    graphics3D_stokes("silo", MyElt , MyGeom , U,V,W,P, "Stokes3D")

elif PROBLEM == "NAVIER-STOKES" :

	 graphics3D( "vtk" , MyElt , MyGeom , U , "NavierStokes3D_U_")
    graphics3D( "vtk" , MyElt , MyGeom , V , "NavierStokes3D_V_")
    graphics3D( "vtk" , MyElt , MyGeom , W , "NavierStokes3D_W_")
    graphics3D( "vtk" , MyEltM1 , GEOM_3D_geomBase_get(MyGeom), P , "NavierStokes3D_P_")
    graphics3D_stokes("vtk", MyElt , MyGeom , U,V,W,P, "NavierStokes3D")

else:

	 print "Problem \"%s\" not yet implemented" % PROBLEM
	 exit(EXIT_FAILURE)

#---------------------------------------------------------------------

# free memory
# -----------

ELT_3D_FREE(MyElt)

GEOM_3D_FREE(MyGeom)
BC_3D_FREE(MyBC)
RHS_3D_FREE(MyRhsFun)

#V_FREE(SOL)

PARAMS_FREE(MyParams)

ELT_3D_FREE(elt_P1)
ELT_3D_FREE(elt_P2)

#---------------------------------------------------------------------

mem_info_file(stdout,0)

mem_info_file(stdout,MY_LIST1)
mem_info_file(stdout,MY_LIST2)
mem_info_file(stdout,MY_LIST3)
mem_info_file(stdout,MY_LIST4)
mem_info_file(stdout,MY_LIST5)
mem_info_file(stdout,MY_LIST6)
mem_info_file(stdout,MY_LIST7) 

fp2.close()

#----------------------------------------------------------------------
