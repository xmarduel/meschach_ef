#! /usr/bin/env python

import sys
import json

from math import *
from time import *
import threading

from meschach         import *
from meschach_spooles import *
from meschach_lapack  import *
from meschach_adds    import *
from meschach_ef      import *
from meschach_cpgplot import *
from meschach_vogle   import *


import time
import profile


#-------------------------------------------------------------------
#
# on a quadra,         ref = 3
#                    *----------*
#                    !          !
#                    !          ! ref = 2
#      ref = 4       !          !
#                    !          !
#                    *----------*
#                      ref = 1
#
# Note: Take care of the references at the 4 corners :
# ----------------------------------------------------
#
# for mesh2D.quad : if BC DIRICHLET everywhere, and  BC NEUMANN on REF=2 or on REF=4, set references like this
#
#----------references sur les sommets------------------
#  4   *nb refs sur les sommets (CL Dirichlet)
#  3 3 3 3 3 3 3 *sommets face nord
#  4 0 0 0 0 0 2 *
#  4 0 0 0 0 0 2 *
#  4 0 0 0 0 0 2 *
#  4 0 0 0 0 0 2 *
#  4 0 0 0 0 0 2 *
#  1 1 1 1 1 1 1 *sommets face sud
#-------------------------------------------------------------------
#
# Note: for mesh2D.quad : if BC DIRICHLET everywhere, and  BC NEUMANN on REF=3 or on REF=1, set references like this
#
#----------references sur les sommets------------------
#  4   *nb refs sur les sommets (CL Dirichlet)
#  4 3 3 3 3 3 2 *sommets face nord
#  4 0 0 0 0 0 2 *
#  4 0 0 0 0 0 2 *
#  4 0 0 0 0 0 2 *
#  4 0 0 0 0 0 2 *
#  4 0 0 0 0 0 2 *
#  4 1 1 1 1 1 2 *sommets face sud
#-------------------------------------------------------------------
#
# => ie at the corners, the references of the points are for the dirichlet bc! 
#

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
def bc_dfdx_p(x,y):
    #return 1.0
    return 10.0/4.0*( - 2.0*(x-0.5) )
    return 2*x*y*y
def bc_dfdx_m(x,y):
    #return 1.0
    return -10.0/4.0*( - 2.0*(x-0.5) )
    return -2*x*y*y
def bc_dfdy_p(x,y):
    #return 1.0
    return 10.0/4.0*( - 2.0*(y-0.5) )
    return 2*x*x*y
def bc_dfdy_m(x,y):
    #return 1.0
    return -10.0/4.0*( - 2.0*(y-0.5) )
    return -2*x*x*y

# --------------------------------------------------------------------
# --------------------------------------------------------------------

def Py_solve2D_eigen(MyElt, MyGeom, MyBC):

    MyParams = Params_get_staticparam(0)

    NBSOMM = GEOM_2D_NBSOMM_get(MyGeom)

    A  = sp_get(NBSOMM,NBSOMM,10)
    M  = sp_get(NBSOMM,NBSOMM,10)


    METHOD = EIGENe_MESCHACH
    #METHOD = EIGENe_SPOOLES
    METHOD = EIGENe_LAPACK

    eig_params = eig_params_get()

    eig_params_set_problem(eig_params, Params_get_oneparam(MyParams,"eigen_params","problem") )
    eig_params_set_option (eig_params, Params_get_oneparam(MyParams,"eigen_params","option_eigen") )
    eig_params_set_ibounds(eig_params, Params_get_oneparam(MyParams,"eigen_params","lower_ibound"), Params_get_oneparam(MyParams,"eigen_params","upper_ibound"))
    eig_params_set_rbounds(eig_params, Params_get_oneparam(MyParams,"eigen_params","lower_rbound"), Params_get_oneparam(MyParams,"eigen_params","upper_rbound"))
    eig_params_set_nb_eigv(eig_params, Params_get_oneparam(MyParams,"eigen_params","nb_eig"))

    # ----- assemblage matrices ------ 

    A = assemblage2D_matrix_Stiff1(MyElt, MyGeom, A)
    M = assemblage2D_matrix_Mass  (MyElt, MyGeom, M)

    transform2D_matrix_with_bc(MyElt, MyGeom, MyBC, A)
    transform2D_matrix_with_bc(MyElt, MyGeom, MyBC, M)

    eigen_values  = v_get(NBSOMM)

    eigen_vectors = sp_eigen(A, M, eigen_values, METHOD, eig_params)

    #P    = px_get(NBSOMM)
    #INVP = px_get(NBSOMM)

    #PAPt  = sp_get(NBSOMM, NBSOMM, 10)
    #PMPt  = sp_get(NBSOMM, NBSOMM, 10)

    #sp_evalpermutation_bandwr(M, P, INVP, BANDWRe_MESCHACH, BANDWRe_MESCHACH_BAND)
    #sp_makepermutation(M, P, INVP, PMPt)
    #sp_makepermutation(A, P, INVP, PAPt)

    #graphics_matrix_pattern ("X11", A, "qq1")
    #graphics_matrix_pattern ("X11", PAPt, "qq2")

    SP_FREE(A)
    SP_FREE(M)

    # finito
    return (eigen_vectors, eigen_values)

# --------------------------------------------------------------------
# --------------------------------------------------------------------

def Py_solve2D_laplacian(MyElt, MyGeom, MyBC, MyRhsFun):

    MyParams = Params_get_staticparam(0)

    NBSOMM = GEOM_2D_NBSOMM_get(MyGeom)

    ResolutionMethod = Params_get_oneparam(MyParams, "matrix_solver_params","resolution_method")
    preconditionning = Params_get_oneparam(MyParams, "matrix_solver_params","preconditionning")
    print("ResolutionMethod =", ResolutionMethod)
    print("Preconditionning =", preconditionning)

    eps_steps = Params_get_oneparam(MyParams, "matrix_solver_params","eps_steps")
    max_steps = Params_get_oneparam(MyParams, "matrix_solver_params","max_steps")
    nb_steps  = new_intp()

    STIFF  = sp_get(NBSOMM, NBSOMM, 10)
    M_BC   = sp_get(NBSOMM, NBSOMM, 10)
    A      = sp_get(NBSOMM, NBSOMM, 10)
    
    RHS_FUN = v_get(NBSOMM)
    RHS_BC  = v_get(NBSOMM)
    RHS     = v_get(NBSOMM)
    
    SOL     = v_get(NBSOMM)

    # ----- assemblage matrix and rhs ------ 

    STIFF = assemblage2D_matrix_Stiff1(MyElt, MyGeom, STIFF)
    M_BC  = assemblage2D_matrix_fromBC(MyElt, MyGeom, MyBC, M_BC)

    A = sp_add(STIFF, M_BC, A)
    
    RHS_FUN  = assemblage2D_vector_fun(MyElt, MyGeom, MyRhsFun, RHS_FUN)
    RHS_BC   = assemblage2D_vector_fromBC(MyElt, MyGeom, MyBC, RHS_BC)
    
    RHS = v_add(RHS_FUN, RHS_BC, RHS)

    transform2D_matrix_vector_with_bc(MyElt, MyGeom, MyBC, A, RHS)


    graphics_matrix_pattern("gnuplot", A, "matrix_profile") # no empty character !

    # ------ solve the system Ax = b   ----- 

    ILU = sp_null()
    ICH = sp_null()

    if  ResolutionMethod == "DIRECT-METHOD" :

        BANDWR_METHOD = Params_get_oneparam(MyParams, "matrix_solver_params", "bandwidth_method")

        t1 = time.time()

        if  BANDWR_METHOD == BANDWRe_AMD:
            spCHresolution_amd(A, RHS, SOL)
        elif BANDWR_METHOD == BANDWRe_SPOOLES:
            spCHresolution_spooles(A, RHS, SOL)
        elif BANDWR_METHOD == BANDWRe_MESCHACH:
            spCHresolution_meschach(A, RHS, SOL)
        else:
            spCHresolution_meschach(A, RHS, SOL)


        t2 = time.time()

        print("\nt2-t1 = ", t2-t1)

        SP_FREE(A)
        V_FREE(RHS)

        return SOL

    # other methods "ITERATIVES"...
    
    if preconditionning == "ICH" :

        ICH = sp_copy(A)
        spICHfactor(ICH)

    elif preconditionning == "ILU" :

        print("for problem = \"Laplacien\", the matrix A should be symmetric")
        print(" -> use preferrably the LLT preconditionning + CG ")

        ILU = sp_copy(A)
        spILUfactor(ILU, 0.0)

    elif preconditionning == "DIAG" :

        print("not implemented -> NULL precond")

    elif preconditionning == "NULL" :

        print("no precond for iterativ method")

    else :

        warnings.warn(WARNINGS_DICT["W_PRECONDITIONNING"])



    if  ResolutionMethod == "CG" :

        iter_xspcg(A, ICH, RHS, eps_steps, SOL, max_steps, nb_steps, None)
        print("cg: # of iter. = ", intp_value(nb_steps))

    elif ResolutionMethod == "BiCGStab" :

        iter_xspbicgstab(A, ILU, RHS, eps_steps, SOL, max_steps, nb_steps, None)
        print("bicgstab: # of iter. = ", intp_value(nb_steps))

    else :

        raise AssertionError, ERRORS_DICT["E_METHOD"]



    #delete_int_p(nb_steps)

    V_FREE(RHS)
    V_FREE(RHS_FUN)
    V_FREE(RHS_BC)

    SP_FREE(A)
    SP_FREE(M_BC)
    SP_FREE(STIFF)

    if preconditionning == "ILU" :
        SP_FREE(ILU)
    if preconditionning == "ICH" or ResolutionMethod == "DIRECT-METHOD" :
        SP_FREE(ICH)

    return SOL

# --------------------------------------------------------------------
# --------------------------------------------------------------------
    
def run_test():
    '''
    '''
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
    err_warn_attach_lists8()

    set_err_flag(EF_JUMP)
    set_warn_flag(EF_SILENT)


    MyBC = Bc2D_get()

    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 3, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 4, AXEe_X )
    #Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 5, AXEe_X ) # or BC_2De_NEUMANN

    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_X, spheric )
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_X, spheric )
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_X, spheric )
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 4, AXEe_X, spheric )
    #Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 5, AXEe_X, spheric )

    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 1, AXEe_X, bc_dfdy_m )
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 2, AXEe_X, bc_dfdx_p )
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 3, AXEe_X, bc_dfdy_p )
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 4, AXEe_X, bc_dfdx_m )
    #Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 5, AXEe_X, bc0 )


    MyRhsFun = Rhs2D_get()
    Rhs2D_setFunctionPython(MyRhsFun, 0, AXEe_X, spheric_src) # ref_e=0, axe=1
    Rhs2D_setFunctionPython(MyRhsFun, 0, AXEe_Y, spheric_src) # ref_e=0, axe=2

    MyAdvFun = Adv2D_get()
    Adv2D_setFunctionPython(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_X, bc_dfdx_p) # ref_e=0, axe=1,1,1
    Adv2D_setFunctionPython(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_Y, bc_dfdx_p) # ref_e=0, axe=1,1,2



    MyParams = Params_get()
    
    config = str(json.dumps( {
        "PDE" : "LAPLACIAN",   # "LAPLACIAN", "CONVECTION-DIFFUSION", "HELMHOLZ",
                            
        "PDE_PARAMATERS" : {
            "KAPPA"    : 1.0,
            "SIGMA"    : 1.0,
            "EPSILON"  : 0.0
        },
                            
        "PDE_RESOLUTION" : {
            "EF" : "P1" ,         # "P1", "P2", "P3"
                            
            "LAPLACIAN_ALGO" : {
                "METHOD" : "ITERATIVE-METHOD",
                            
                "ITERATIVE-METHOD" : {
                    "METHOD"          : "CG" ,       # "CG", "CGS", "BiCGStab"
                    "PRECONDITIONER"  : "NULL" ,     # "NULL", "ICH", "ILU", "DIAG"
                    "PARAMETERS" : {
                        "NMAX_ITER" : 500,
                        "EPSILON"   : 1.0e-12
                    },
                    "OUTPUT" : {
                        "STDOUT" : True,
                        "FILE"   : "residuals.dat"
                    }
                },
                            
                "DIRECT-METHOD" : {
                    "BANDWIDTHREDUCTION" : {
                        "OPTION"         : "BAND",  # "PROFILE"
                        "IMPLEMENTATION" : "MESCHACH"
                    }
                }
            },
                            
            # parameter for the resolution of the eigenvalues problem ------------------------------------
            "HELMHOLZ_ALGO" : {
                "EIGEN_PROBLEM" : "GENERALIZED_SYMMETRIC_1",
                "EIGEN_OPTION"  : "RBOUNDS",
                                                    
                "NB_EIGENS"     : 10,
                                                    
                "LOWER_IBOUND"  : 50,
                "UPPER_IBOUND"  : 100,
                                                    
                "LOWER_RBOUND"  : 1.1,
                "UPPER_RBOUND"  : 500.0
            }
        },
                           
        "OUTPUT_LOG_FILE"    : "output.txt"
                            
    } ))
                            
    MyParams = Params_append_from_string(MyParams, config)

    # geom data
    config = str(json.dumps( {
        "COORDS_DIMS"    : 2,

        "MESH_2D" : {
            "MESHFILEDEFINITION" : {
                "MESHFILE" : "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/2D/mesh2D.quad",
                "MESHNAME" : "MESH_40",     # "MESH_03", "MESH_04", "MESH_05", "MESH_06", "MESH_10", "MESH_20", "MESH_40", "MESH_50"
                "MESHTYPE" : "quad1"        # "quad1", "quad2", "quad3"
            
                # "MESHFILE" : "/Users/xavier/DEVELOPPEMENT/MESCHACH_WORK/EF_MESHES/2D/CUBE_11.emc2",
                # "MESHNAME" : null,
                # "MESHTYPE" : "emc2"
               
                # "MESHFILE" : "/Users/xavier/DEVELOPPEMENT/MESCHACH_WORK/EF_MESHES/2D/cube2D_P1_v2.gmsh",
                # "MESHNAME" : null,
                # "MESHTYPE" : "gmsh"
            },
        }
    } ))
   
    MyParams = Params_append_from_string(MyParams, config)

    # plotting data
    config = str(json.dumps( {
        "COORDS_DIMS"    : 2,
        "GRAPHICS_2D_OUTPUT" : {
            "GNUPLOT"    : False,
            "MATLAB"     : False,
            "VTK"        : False,
            "SILO"       : True,
            "VOGLE"      : True,
            "MATPLOTLIB" : False
        },
    } ))
   
    MyParams = Params_append_from_string(MyParams, config)

    # plotting data
    config = str(json.dumps( {
         "COORDS_DIMS"    : 2,
         "GRAPHICS_2D" : {
             "DRIVER"      : "/XWINDOW" ,
             "WINDOW_SIZE" : [700, 500] ,
             "LABELS_SIZE" : 1.5,
             "SUB_WINDOWS" : [
                {
                   "IDXS"         : [1,1] ,
                   "VIEWPORT"     : { "X_MIN":0.0, "X_MAX":1.0, "Y_MIN":0.0 , "Y_MAX":1.0 },
                             
                    "GRAPHICS_TYPE"    : "CONTOURS",
                              #"GRAPHICS_TYPE"    : "SURFACE",
                             
                    "LEGEND"       : "contours" ,
                             
                    "CONTOURS"     : {
                       "NB_LEVELS"    : 11 ,
                        "LEVELS"       : [0.1, 0.2, 0.3, 0.7, 0.9, 1.1 ]
                    },
                             
                    "SURFACE"      : {
                      "DUMMY"    : True
                    }
                 }
             ]
         }
    } ))
   
    MyParams = Params_append_from_string(MyParams, config)



    Params_set_staticparam(MyParams, 0)


    MyElt  = elt2D_get(Params_get_oneparam(MyParams, "finite_elements_params", "name_ef"))

    elt_P1 = elt2D_get("P1")
    elt_P2 = elt2D_get("P2")
    elt_P3 = elt2D_get("P3")

    MyGeom = Geom2D_get(MyElt,
            Params_get_oneparam(MyParams, "geometry_params", "meshfile"),
            Params_get_oneparam(MyParams, "geometry_params", "meshname"),
            Params_get_oneparam(MyParams, "geometry_params", "meshtype"))

    Geom2D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X)


    PROBLEM = Params_get_oneparam(MyParams, "main_problem", "NULL")
    print("PROBLEM is ",PROBLEM)


    if   PROBLEM == "HELMHOLZ" :

        (eigen_vectors, eigen_values) = Py_solve2D_eigen(MyElt, MyGeom, MyBC)

    elif PROBLEM == "LAPLACIAN" :

        SOL = Py_solve2D_laplacian(MyElt, MyGeom, MyBC, MyRhsFun)

    elif PROBLEM == "CONVECTION-DIFFUSION" :

        SOL = solve2D_convection_diffusion(MyElt, MyGeom, MyBC, MyRhsFun, MyAdvFun)

    elif PROBLEM == "STOKES" :

        (U,V,P) = Py_solve2D_stokes(elt_P2, elt_P1, MyGeom, MyBC, MyRhsFun)

    elif PROBLEM == "NAVIER-STOKES" :

        (U,V,P) = Py_solve2D_navier_stokes(elt_P2, elt_P1, MyGeom, MyBC, MyRhsFun)

    else:

        print("Problem \"%s\" not yet implemented" % PROBLEM)
        exit(EXIT_FAILURE)



    #----------------------- graphics output -----------------------------

    if   PROBLEM == "HELMHOLZ" :

        print("DIM 1=", MAT_m_get(eigen_vectors))
        print("DIM 2=", MAT_n_get(eigen_vectors))
        v_foutput(sys.stdout, eigen_values)
        graphics2D_set("gnuplot" , MyElt , MyGeom , MAT_n_get(eigen_vectors), eigen_vectors, "SolEigen" )
        graphics2D_set("silo"    , MyElt , MyGeom , MAT_n_get(eigen_vectors), eigen_vectors, "SolEigen" )

    elif PROBLEM == "LAPLACIAN" :

        graphics2D( "gnuplot" , MyElt , MyGeom , SOL , "SolApproch2D")
        graphics2D( "silo"    , MyElt , MyGeom , SOL , "SolApproch2D")
        graphics2D( "vtk"     , MyElt , MyGeom , SOL , "SolApproch2D")

     
        if Params_get_oneparam2(MyParams, "graphics_interactiv2D_params", "GRAPHICS_TYPE", 1, 1) == "SURFACE":
            
            graphics2D_vogle_initialize(Params_get_oneparam(MyParams,"graphics_interactiv2D_params","DRIVER"),
                                        Params_get_oneparam(MyParams,"graphics_interactiv2D_params","SIZE_WINDOW_X"),
                                        Params_get_oneparam(MyParams,"graphics_interactiv2D_params","SIZE_WINDOW_Y"))

            graphics2D_vogle_title("SOLUTION")
            graphics2D_vogle_store_surfacedata(GEOM_2D_NSELMT_get(MyGeom), GEOM_2D_XYSOMM_get(MyGeom))
            graphics2D_vogle_store_plotdata(SOL)
                
            graphics2D_vogle_plotsurface()
    
        else:
            graphics1D_vopl_initialize(Params_get_oneparam(MyParams,"graphics_interactiv2D_params","DRIVER"),
                                       Params_get_oneparam(MyParams,"graphics_interactiv2D_params","SIZE_WINDOW_X"),
                                       Params_get_oneparam(MyParams,"graphics_interactiv2D_params","SIZE_WINDOW_Y"),
                                       1, 1, VOPL_CONTOURS)
            graphics1D_vopl_title("SOLUTION")
            graphics1D_vopl_window(1,1, Params_get_oneparam2(MyParams,"graphics_interactiv2D_params","WINDOW_X_MIN",1,1),
                                        Params_get_oneparam2(MyParams,"graphics_interactiv2D_params","WINDOW_X_MAX",1,1),
                                        Params_get_oneparam2(MyParams,"graphics_interactiv2D_params","WINDOW_Y_MIN",1,1),
                                        Params_get_oneparam2(MyParams,"graphics_interactiv2D_params","WINDOW_Y_MAX",1,1))

            graphics1D_vopl_nblevels(1, 1, Params_get_oneparam2(MyParams,"graphics_interactiv2D_params","NB_LEVELS",1,1))
            graphics1D_vopl_levels(1, 1, Params_get_oneparam2(MyParams,"graphics_interactiv2D_params","LEVELS",1,1))
            graphics1D_vopl_contourmeshdata(1, 1, GEOM_2D_NSELMT_get(MyGeom), GEOM_2D_XYSOMM_get(MyGeom))
            graphics1D_vopl_contourplotdata(1, 1, SOL)

            t1 = threading.Thread( target=Py_vopl_contour_with_xt_toolkit )
            t1.start()

    elif PROBLEM == "CONVECTION-DIFFUSION" :

        graphics2D( "gnuplot" , MyElt , MyGeom , SOL , "SolApproch2D")
        graphics2D( "silo"    , MyElt , MyGeom , SOL , "SolApproch2D")
        graphics2D( "vtk"     , MyElt , MyGeom , SOL , "SolApproch2D")

        graphics2D_vogle_initialize(Params_get_oneparam(MyParams,"graphics_interactiv2D_params","DRIVER"),
                                    Params_get_oneparam(MyParams,"graphics_interactiv2D_params","SIZE_WINDOW_X"),
                                    Params_get_oneparam(MyParams,"graphics_interactiv2D_params","SIZE_WINDOW_Y"))
                
        graphics2D_vogle_title("SOLUTION")
        graphics2D_vogle_store_surfacedata(GEOM_2D_NSELMT_get(MyGeom), GEOM_2D_XYSOMM_get(MyGeom))
        graphics2D_vogle_store_plotdata(SOL)
                
        graphics2D_vogle_plotsurface()

    elif PROBLEM == "STOKES" :

        graphics2D_stokes( "vtk", MyElt , MyGeom , U, V, P, "Stokes2D")

    elif PROBLEM == "NAVIER-STOKES" :

        graphics2D_stokes( "vtk", MyElt , MyGeom , U, V, P, "NavierStokes2D")

    else:

        print("Problem \"%s\" not yet implemented" % PROBLEM)
        exit(EXIT_FAILURE)


    #---------------------------------------------------------------------
    # file output

    fp1 = open("sol_2.dat", "w")
    v_foutput(fp1,SOL)
    fp1.close()

    fp2 = open("xysomm_2.dat", "w")
    m_foutput(fp2,GEOM_2D_XYSOMM_get(MyGeom))
    fp2.close()

    fp3 = open("nselmt_2.dat", "w")
    im_foutput(fp3, GEOM_2D_NSELMT_get(MyGeom))
    fp3.close()

    #---------------------------------------------------------------------

    if PROBLEM == "LAPLACIAN" :
    
        fun2D = Fun2D_get()
        Fun2D_setFunctionPython(fun2D, spheric) # ref_e=0, axe=1

        NBSOMM = GEOM_2D_NBSOMM_get(MyGeom)

        SOL_exact = v_get(NBSOMM)
        SOL_exact = build_vec_from_function2D(MyElt, MyGeom, fun2D, SOL_exact)
      
        print("diff solexacte-solapprochee = %le" % v_norm2( v_sub(SOL, SOL_exact, None) ))

        FUN_2D_FREE(fun2D)

    #-------------------------------------------------------------------

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

    #M_FREE(eigen_vectors)
    
    #V_FREE(eigen_values)

    #mem_info_file(stdout, 0)

    #mem_info_file(stdout, MY_LIST1)
    #mem_info_file(stdout, MY_LIST2)
    #mem_info_file(stdout, MY_LIST3)
    #mem_info_file(stdout, MY_LIST4)
    #mem_info_file(stdout, MY_LIST5)
    #mem_info_file(stdout, MY_LIST6)
    #mem_info_file(stdout, MY_LIST7)

    #----------------------------------------------------------------------
    Py_pthread_exit()



if __name__ == '__main__':
    if len(sys.argv) > 1:
        profile.run('run_test()')
    else:
        run_test()
