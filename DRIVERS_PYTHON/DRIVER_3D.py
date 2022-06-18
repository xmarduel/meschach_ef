#! /usr/bin/env python


from sys import *
from math import *

from meschach         import *
from meschach_adds    import *
from meschach_ef      import *
from meschach_cpgplot import *
from meschach_spooles import *

import warnings
import time


ERRORS_DICT = { 
    "E_UNKNOWN" : "unknowm error !!!" ,
    "E_METHOD"  : "unknown method !!!"
}
WARNINGS_DICT = { 
    "W_UNKNOWN"           : "unknowm warning !"   ,
    "W_PRECONDITIONNING"  : "unknown preconditionning !" 
}

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
set_warn_flag(EF_SILENT)

#-------------------------------------------------------------------


def solution(x,y,z):
    #return x*x*y
    #return x + 2*y + 3*z + x*x
    #return x + 2*y
    #return x + 2*y + 3*z 
    return 10.0/6.0 * (0.5 - (x-0.5)*(x-0.5) - (y-0.5)*(y-0.5) - (z-0.5)*(z-0.5) ) 
def source(x,y,z):
    #return -2*y
    #return -2
    #return 0
    #return 0
    return 10.0

def Dxsolution(x,y,z):
    #return 2*x*y
    #return 1 + 2*x
    #return 1
    return 1
    return 10.0/6.0*( -2*(x-0.5) )
	 
def minusDxsolution(x,y,z):
    return - Dxsolution(x,y,z)

def Dysolution(x,y,z):
    #return 2*x*y
    #return 1 + 2*x
    #return 2
    return 2
    return 10.0/6.0*( -2*(y-0.5) )

def minusDysolution(x,y,z):
    return - Dysolution(x,y,z)
	 
def Dzsolution(x,y,z):
    #return 2*x*y
    #return 1 + 2*x
    #return 0
    return 3
    return 10.0/6.0*( -2*(z-0.5) )

def minusDzsolution(x,y,z):
    return - Dzsolution(x,y,z)
	 
def stream_u(x,y,z):
    return x
def stream_v(x,y,z):
    return y
def stream_w(x,y,z):
    return z


MyBC = Bc3D_get()

Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 1, AXEe_X ) # TOP_FACE
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 2, AXEe_X ) # BOTTOM_FACE
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 3, AXEe_X ) # FRONT_FACE
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 4, AXEe_X ) # BACK_FACE
#Bc3D_setBcType(MyBC, BC_3De_NEUMANN, 5, AXEe_X ) # RIGHT_FACE : or BC_3De_NEUMANN
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 5, AXEe_X ) # RIGHT_FACE : or BC_3De_NEUMANN
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 6, AXEe_X ) # LEFT_FACE

Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 1, AXEe_X, solution)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 2, AXEe_X, solution)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 3, AXEe_X, solution)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 4, AXEe_X, solution)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 5, AXEe_X, solution)
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 6, AXEe_X, solution)

Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 1, AXEe_X, Dzsolution)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 2, AXEe_X, minusDzsolution)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 3, AXEe_X, minusDysolution)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 4, AXEe_X, Dysolution)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 5, AXEe_X, Dxsolution)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 6, AXEe_X, minusDxsolution)



MyRhsFun = Rhs3D_get()
Rhs3D_setFunctionPython(MyRhsFun, 0, AXEe_X, source) # ref_e=0, axe=1
Rhs3D_setFunctionPython(MyRhsFun, 0, AXEe_Y, source) # ref_e=0, axe=2
Rhs3D_setFunctionPython(MyRhsFun, 0, AXEe_Z, source) # ref_e=0, axe=3


MyAdvFun = Adv3D_get()
Adv3D_setFunctionPython(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_X, source) # ref_e=0, eq1,unknown1,dx
Adv3D_setFunctionPython(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_Y, source) # ref_e=0, eq1,unknown1,dy
Adv3D_setFunctionPython(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_Z, source) # ref_e=0, eq1,unknown1,dz

#Rhs3D_setCFunction(MyRhsFun, 0, AXEe_X, Src3D_08) # ref_e=0, axe=1 #Phi3D_08, Src3D_08
#-------------------------------------------------------------------   

fp2 = open("OUTPUT.txt", "w")

MyParams = Params_get()

#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "input_output_files","fp2", fp2)
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "main_problem","NULL", "LAPLACIAN" ) # "LAPLACIAN" "STOKES" "NAVIER-STOKES" "CONVECTION-DIFFUSION"
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "finite_elements_params","name_ef", "P2" )    # Type d'EF : "P1","P2"
#
Params_set_oneparam(MyParams, "matrix_solver_params","resolution_method", "CG" )     # Methode : DIRECT-METHOD,CG,CGS,BiCGStab
Params_set_oneparam(MyParams, "matrix_solver_params","preconditionning", "NULL" )    # Precond : NULL, ICH, ILU, DIAG. 

#Params_set_oneparam(MyParams, "geometry_params","meshfile", "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/3D/mesh3D.cube" )  #  Mesh File ("cube3D_P1_v6.gmsh"  "cube3D_P1.cube" )
#Params_set_oneparam(MyParams, "geometry_params","meshfile", "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/3D/cube3D_P1_v6.gmsh" )  #  Mesh File ("cube3D_P1_v6.gmsh")
Params_set_oneparam(MyParams, "geometry_params","meshfile", "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/3D/cube3D_P1_v7.gmsh" )  #  Mesh File ("cube3D_P1_v6.gmsh") 
#Params_set_oneparam(MyParams, "geometry_params","meshtype", "cube" )
Params_set_oneparam(MyParams, "geometry_params","meshtype", "gmsh" )  #  
Params_set_oneparam(MyParams, "geometry_params","meshname", "MESH_10" )  #  

Params_set_oneparam(MyParams, "graphics_output_params","GNUPLOT",     0) # GNUPLOT
Params_set_oneparam(MyParams, "graphics_output_params","GRAPH",       0) # GRAPH
Params_set_oneparam(MyParams, "graphics_output_params","MATLAB",      0) # MATLAB
Params_set_oneparam(MyParams, "graphics_output_params","VTK",         1) # VTK
Params_set_oneparam(MyParams, "graphics_output_params","SILO",        1) # SILO
Params_set_oneparam(MyParams, "graphics_output_params","PGPLOT",      0) # PGPLOT
Params_set_oneparam(MyParams, "graphics_output_params","VOGLE",       0) # VOGLE
Params_set_oneparam(MyParams, "graphics_output_params","LIBSCIPLOT",  0) # LIBSCIPLOT   (0=NO1=YES)

#-physical parameters------------------------------------------------------------------

Params_set_oneparam(MyParams, "physical_params","epsilon",    0.0   ) # epsilon
Params_set_oneparam(MyParams, "physical_params","sigma",      0.0   ) # sigma
Params_set_oneparam(MyParams, "physical_params","kappa",      1.0   ) # kappa  -k*LAPLACIEN(u)+ e.BILAPL(u) +sigma.u =F


#-parameter for the resolution of the eigenvalues problem ------------------------------------

Params_set_oneparam(MyParams, "eigen_params", "problem"     , EIGENe_GENERALIZED_SYMMETRIC_1)
Params_set_oneparam(MyParams, "eigen_params", "option_eigen", EIGENe_RBOUNDS)
Params_set_oneparam(MyParams, "eigen_params", "nb_eig"      , 50)
Params_set_oneparam(MyParams, "eigen_params", "lower_ibound", 1)
Params_set_oneparam(MyParams, "eigen_params", "upper_ibound", 40)
Params_set_oneparam(MyParams, "eigen_params", "lower_rbound", 1.1)
Params_set_oneparam(MyParams, "eigen_params", "upper_rbound", 500.0)
#  
#-parameter for the resolution of the linear system------------------------------------

Params_set_oneparam(MyParams, "matrix_solver_params","max_steps", 1000   ) # max_iter Nombre max d'iter de la methode iterative
Params_set_oneparam(MyParams, "matrix_solver_params","eps_steps", 1.e-14) # eps_iter arret des iterations "  "  "   "

Params_set_oneparam(MyParams, "matrix_solver_params","iter_info", ITER_INFOe_ALL   )     # iter_info ( ITER_INFOe_[NULL-STDOUT-FILE-ALL] )
Params_set_oneparam(MyParams, "matrix_solver_params","iter_file", "the_residuals.dat"  ) # iter_file le nom du fichier

Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_SPOOLES)   # BANDWRe_MESCHACH ** BANDWRe_SPOOLES ** BANDWRe_AMD
Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_option", BANDWRe_PROFILE)   # USE BAND WIDTH REDUCTION


#############################################################################################
#Params_fill_allparams(MyParams, "INPUT_PDE_2D.dat", "OUTPUT_PDE_2D.dat", lecture_params2D )
#############################################################################################
Params_set_staticparam(MyParams, 0)


MyElt  = elt3D_get(Params_get_oneparam(MyParams, "finite_elements_params", "name_ef"))

elt_P2 = elt3D_get("P2")
elt_P1 = elt3D_get("P1")

#---------------------------------------------------------------------------------------------

MyGeom = Geom3D_get(MyElt,        
        Params_get_oneparam(MyParams, "geometry_params", "meshfile"),
        Params_get_oneparam(MyParams, "geometry_params", "meshname"),
        Params_get_oneparam(MyParams, "geometry_params", "meshtype"))

#graphics_geom3D_view("gnuplot", MyGeom, "geometry3D_view")
#Geom3D_foutput(stdout, MyGeom)

#graphics_geom3D_view("gnuplot", MyGeom, "cube3D_view")

#MyGeomTetra = Geom3D_get_base_tetrahedra(3)
#graphics_geom3D_view("gnuplot", MyGeomTetra, "tetra3D_view")
#Geom3D_foutput(stdout, MyGeomTetra)

#exit(0)

Geom3D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X)  

#---------------------------------------------------------------------------------------------

PROBLEM = Params_get_oneparam(MyParams, "main_problem", "NULL")
print(PROBLEM)

# --------------------------------------------------------------------
# --------------------------------------------------------------------

def Py_solve3D_eigen( MyElt , MyGeom , MyBC ) :

    NBSOMM = GEOM_3D_NBSOMM_get(MyGeom)


    A  = sp_get(NBSOMM,NBSOMM,10)
    M  = sp_get(NBSOMM,NBSOMM,10)


    METHOD = EIGENe_MESCHACH
    #METHOD = EIGENe_SPOOLES
    METHOD = EIGENe_LAPACK

    eig_params = eig_params_get()

    eig_params_set_problem(eig_params, EIGENe_GENERALIZED_SYMMETRIC_1)
    eig_params_set_option (eig_params, EIGENe_RBOUNDS)
    eig_params_set_ibounds(eig_params, Params_get_oneparam(MyParams,"eigen_params","lower_ibound"), Params_get_oneparam(MyParams,"eigen_params","upper_ibound"))
    eig_params_set_rbounds(eig_params, Params_get_oneparam(MyParams,"eigen_params","lower_rbound"), Params_get_oneparam(MyParams,"eigen_params","upper_rbound"))
    eig_params_set_nb_eigv(eig_params, Params_get_oneparam(MyParams,"eigen_params","nb_eig"))


    # ----- assemblage matrices ------ 

    A = assemblage3D_matrix_Stiff1( MyElt , MyGeom , A )
    M = assemblage3D_matrix_Mass  ( MyElt , MyGeom , M )

    transform3D_matrix_with_bc( MyElt , MyGeom , MyBC , A )
    transform3D_matrix_with_bc( MyElt , MyGeom , MyBC , M )

    eigen_values  = v_get(NBSOMM)

    eigen_vectors = sp_eigen(A, M, eigen_values, METHOD, eig_params )


    SP_FREE(A)
    SP_FREE(M)

    # finito
    return (eigen_vectors, eigen_values)

# --------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------

def Py_solve3D_laplacien( MyElt , MyGeom , MyBC , MyRhsFun ) :

    NBSOMM = GEOM_3D_NBSOMM_get(MyGeom)

    resolution_method = Params_get_oneparam(MyParams,"matrix_solver_params","resolution_method")
    preconditionning = Params_get_oneparam(MyParams,"matrix_solver_params","preconditionning")
    print("resolutionMethod =", resolution_method)
    print("preconditionning =", preconditionning)

    BandwrMethod = Params_get_oneparam(MyParams,"matrix_solver_params","bandwidth_method")
    BandwrOption = Params_get_oneparam(MyParams,"matrix_solver_params","bandwidth_option")
    print("BandwrMethod =", BandwrMethod)
    print("BandwrOption =", BandwrOption)

    eps_steps = Params_get_oneparam(MyParams,"matrix_solver_params","eps_steps")
    max_steps = Params_get_oneparam(MyParams,"matrix_solver_params","max_steps")
    nb_steps  = new_intp()

    A       = sp_get(NBSOMM,NBSOMM,10)
    M_STIFF = sp_get(NBSOMM,NBSOMM,10)
    M_BC    = sp_get(NBSOMM,NBSOMM,10)

    RHS     = v_get(NBSOMM)
    RHS_FUN = v_get(NBSOMM)
    RHS_BC  = v_get(NBSOMM)
    
    VIT = v_get(NBSOMM)

    # ----- assemblage matrix and rhs ------ 

    M_STIFF = assemblage3D_matrix_Stiff1( MyElt , MyGeom , M_STIFF )
    M_BC    = assemblage3D_matrix_fromBC( MyElt , MyGeom , MyBC, M_BC )

    RHS_FUN  = assemblage3D_vector_fun( MyElt , MyGeom , MyRhsFun, RHS_FUN )
    RHS_BC   = assemblage3D_vector_fromBC( MyElt , MyGeom , MyBC, RHS_BC )

    A = sp_add(M_STIFF, M_BC, A)
    RHS = v_add(RHS_FUN, RHS_BC, RHS)
    
    transform3D_matrix_vector_with_bc( MyElt , MyGeom , MyBC , A , RHS )

    # ------ solve the system Ax = b   ----- 

    ILU = sp_null()
    ICH = sp_null()

    if  resolution_method == "DIRECT-METHOD" :
   
        t1 = time.time()
    
        if   ( BandwrMethod == BANDWRe_AMD ) :
            spCHresolution_amd(A, RHS, VIT)
        elif ( BandwrMethod == BANDWRe_SPOOLES ) :
            spCHresolution_spooles(A, RHS, VIT)
        else:
            spCHresolutione_meschach(A, RHS, VIT)
         
        t2 = time.time()
    
        print("\nt2-t1 = ", t2-t1)
    
        RESIDU = v_get(NBSOMM)
        sp_mv_mlt(A,VIT,RESIDU)
        v_sub(RESIDU,RHS,RESIDU)

        print("\nnorm res = ",v_norm2(RESIDU))

        V_FREE(RESIDU)


        return VIT

    # then iterative method
    if preconditionning == "ICH" :

        ICH = sp_copy(A)
        spICHfactor(ICH)    

    elif preconditionning == "ILU" :

        print("for problem = \"Laplacien\", the matrix A should be symmetric")
        print("-> use preferrably the LLT preconditionning + CG ")

        ILU = sp_copy(A)
        spILUfactor(ILU, 0.0)    

    elif preconditionning == "DIAG" :

        print("not implemented -> NULL precond")

    elif preconditionning == "NULL" :

        print("no precond for iterativ method")

    else :

        warnings.warn(WARNINGS_DICT["W_PRECONDITIONNING"])



    if  resolution_method == "CG" :

        iter_xspcg(A, ICH, RHS, eps_steps, VIT, max_steps, nb_steps, None)
        print("cg: # of iter. = %d" % intp_value(nb_steps))

    elif resolution_method == "BiCGStab" :

        iter_xspbicgstab(A, ILU, RHS, eps_steps, VIT, max_steps, nb_steps, None)
        print("bicgstab: # of iter. = %d" % intp_value(nb_steps))

    else :

        raise AssertionError(ERRORS_DICT["E_METHOD"])


     
    delete_intp(nb_steps)

    V_FREE(RHS)
    SP_FREE(A) 

    if preconditionning == "ILU" :
        SP_FREE(ILU)
    if preconditionning == "ICH" or resolution_method == "DIRECT-METHOD" :
        SP_FREE(ICH)


    return VIT


# ---------------------------------------------------------------------------------------

if   PROBLEM == "LAPLACIAN" :

    #SOL = solve3D_laplacian( MyElt , MyGeom , MyBC , MyRhsFun )
    SOL = Py_solve3D_laplacien( MyElt , MyGeom , MyBC , MyRhsFun )

elif PROBLEM == "CONVECTION-DIFFUSION" :

	SOL = solve3D_convection_diffusion( MyElt , MyGeom , MyBC , MyRhsFun , MyAdvFun )

elif PROBLEM == "STOKES" :

	(U,V,W,P) = Py_solve3D_stokes( elt_P2 , elt_P1 , MyGeom , MyBC , MyRhsFun )

elif PROBLEM == "NAVIER-STOKES" :

	(U,V,W,P) = Py_solve3D_navier_stokes( elt_P2 , elt_P1 , MyGeom , MyBC , MyRhsFun )

else:

    print("Problem \"%s\" not yet implemented" % PROBLEM)
    exit(EXIT_FAILURE)



#----------------------- graphics output -----------------------------


if    PROBLEM == "LAPLACIAN"  :

    graphics3D( "vtk"  , MyElt , MyGeom , SOL , "SolApproch3D")
    graphics3D( "silo" , MyElt , MyGeom , SOL , "SolApproch3D")

elif PROBLEM == "CONVECTION-DIFFUSION" :

    graphics3D( "vtk"  , MyElt , MyGeom , SOL , "SolApproch3D")
    graphics3D( "silo" , MyElt , MyGeom , SOL , "SolApproch3D")

elif PROBLEM == "STOKES" :

    graphics3D( "vtk" , MyElt , MyGeom , U , "Stokes_U_Approch3D")
    graphics3D( "vtk" , MyElt , MyGeom , V , "Stokes_V_Approch3D")
    graphics3D( "vtk" , MyElt , MyGeom , W , "Stokes_W_Approch3D")

elif PROBLEM == "NAVIER-STOKES" :

    graphics3D( "vtk" , MyElt , MyGeom , U , "NavierStokes_U_Approch3D")
    graphics3D( "vtk" , MyElt , MyGeom , V , "NavierStokes_V_Approch3D")
    graphics3D( "vtk" , MyElt , MyGeom , W , "NavierStokes_W_Approch3D")

else:

    print("Problem \"%s\" not yet implemented" % PROBLEM)
    exit(EXIT_FAILURE)


#---------------------------------------------------------------------
has_exact_sol = True

if has_exact_sol == True :

    fun3D = Fun3D_get()
    Fun3D_setFunctionPython(fun3D, solution) # ref_e=0, axe=1

    NBSOMM = GEOM_3D_NBSOMM_get(MyGeom)
         
    SOL_exact = v_get(NBSOMM)
    SOL_exact = build_vec_ef_from_function3D(MyElt, MyGeom, fun3D, SOL_exact)
               
    print("diff solexacte-solapprochee = %le" % v_norm2( v_sub(SOL, SOL_exact, None) ))
                     
    FUN_3D_FREE(fun3D)

    graphics3D( "vtk",  MyElt , MyGeom , SOL , "Diff_Approch3D")

  
# free memory
# -----------

ELT_3D_FREE(MyElt)

GEOM_3D_FREE(MyGeom)
BC_3D_FREE(MyBC)
RHS_3D_FREE(MyRhsFun)
ADV_3D_FREE(MyAdvFun)

V_FREE(SOL)

PARAMS_FREE(MyParams)

ELT_3D_FREE(elt_P1)
ELT_3D_FREE(elt_P2)

#---------------------------------------------------------------------

#mem_info_file(stdout, 0)

#mem_info_file(stdout, MY_LIST1)
#mem_info_file(stdout, MY_LIST2)
#mem_info_file(stdout, MY_LIST3)
#mem_info_file(stdout, MY_LIST4)
#mem_info_file(stdout, MY_LIST5)
#mem_info_file(stdout, MY_LIST6)
#mem_info_file(stdout, MY_LIST7)

fp2.close()

#----------------------------------------------------------------------
