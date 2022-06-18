#! /usr/bin/env python
#
import sys
from   math  import cos, sin, sqrt, pi, exp
import time
import threading
#
from meschach         import *
from meschach_spooles import *
from meschach_lapack  import *
from meschach_adds    import *
from meschach_ef      import *
from meschach_cpgplot import *
from meschach_vogle   import *
#
#
import time
import profile
#
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
#                    !     ! ref = 3
#                    !     * 
#                    !     !
#       ref = 2      !      \
#                    !          \
#                    !             * 
#                    !             ! ref = 2
#                    !             !
#                    *-------------*
#                      ref = 2
#
#
#
#
#
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



def laplacian_src(x,y):
    return 20.0

def bc_dirichlet(x,y):
    return 0.0
def bc_neumann(x,y):
    return 0.0
def bc_cauchy(x,y):
    return 2.0


MyBC = Bc2D_get()

Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_X )
Bc2D_setBcType(MyBC, BC_2De_NEUMANN,   2, AXEe_X )
Bc2D_setBcType(MyBC, BC_2De_CAUCHY,    3, AXEe_X )

Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_X, bc_dirichlet )
Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN,   2, AXEe_X, bc_neumann )
Bc2D_setFunctionPython(MyBC, BC_2De_CAUCHY,    3, AXEe_X, bc_cauchy )

MyRhsFun = Rhs2D_get()
Rhs2D_setFunctionPython(MyRhsFun, 0, AXEe_X, laplacian_src) # ref_e=0, axe=1


#-------------------------------------------------------------------   
#
# LECTURE  IN  INPUT FILE "INPUT_PDE.dat"
# ECRITURE IN OUTOUT FILE "OUTPUT_PDE.dat"
#      
fp2 = open("OUTPUT.txt", "w")

MyParams = Params_get()
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "input_output_files", "fp2", fp2)
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "main_problem","NULL", "LAPLACIAN" )
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "finite_elements_params","name_ef", "P1" )    # Type d'EF : "P1","P2", "P3"

Params_set_oneparam(MyParams, "matrix_solver_params","resolution_method", "CG" )     # Methode : DIRECT-METHOD,CG,CGS,GMRES(k) 
Params_set_oneparam(MyParams, "matrix_solver_params","preconditionning", "NULL" )   # Precond : NULL, ICH, ILU, DIAG. 

Params_set_oneparam(MyParams, "geometry_params","meshfile", "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/2D/schwarz.msh" )  #  Mesh File ("name.dat")
Params_set_oneparam(MyParams, "geometry_params","meshtype", "gmsh" )  #  Mesh File ("name.dat")
Params_set_oneparam(MyParams, "geometry_params","meshname", "" )  #  Mesh File ("name.dat")

Params_set_oneparam(MyParams, "graphics_output_params","GNUPLOT",     0) # GNUPLOT
Params_set_oneparam(MyParams, "graphics_output_params","GRAPH",       0) # GRAPH
Params_set_oneparam(MyParams, "graphics_output_params","MATLAB",      0) # MATLAB
Params_set_oneparam(MyParams, "graphics_output_params","VTK",         1) # VTK
Params_set_oneparam(MyParams, "graphics_output_params","SILO",        1) # SILO
Params_set_oneparam(MyParams, "graphics_output_params","PGPLOT",      0) # PGPLOT
Params_set_oneparam(MyParams, "graphics_output_params","VOGLE",       1) # VOGLE
Params_set_oneparam(MyParams, "graphics_output_params","LIBSCIPLOT",  0) # LIBSCIPLOT   (0=NO1=YES)

#-parameter for the graphics------------------------------------

Params_set_oneparam(MyParams, "graphics_interactiv2D_params","DRIVER", "/XWINDOW"  )
Params_set_oneparam(MyParams, "graphics_interactiv2D_params","SIZE_WINDOW_X", 700 )
Params_set_oneparam(MyParams, "graphics_interactiv2D_params","SIZE_WINDOW_Y", 500 )
Params_set_oneparam(MyParams, "graphics_interactiv2D_params","SIZE_LABELS", 1.1  )

Params_set_oneparam2(MyParams, "graphics_interactiv2D_params","LABEL_LEGEND", 1,1, "Solution"  )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params","GRAPHICS_TYPE", 1,1, "CONTOURS"  )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params","WINDOW_X_MIN", 1,1,  0.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params","WINDOW_X_MAX", 1,1, 10.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params","WINDOW_Y_MIN", 1,1,  0.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params","WINDOW_Y_MAX", 1,1, 10.0  )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params","NB_LEVELS"   , 1,1, 7    )
Params_set_oneparam2(MyParams, "graphics_interactiv2D_params","LEVELS"      , 1,1, [10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0, 120.0, 130.0, 140.0, 150.0]  )



#-physical parameters------------------------------------------------------------------

Params_set_oneparam(MyParams, "physical_params","epsilon",    0.0000 ) # epsilon
Params_set_oneparam(MyParams, "physical_params","sigma",      0.0    ) # sigma
Params_set_oneparam(MyParams, "physical_params","kappa",      1.000  ) # kappa  -k*LAPLACIEN(u)+ e.BILAPL(u) +sigma.u =F

#-parameter for the resolution of the linear system------------------------------------

Params_set_oneparam(MyParams, "matrix_solver_params","max_steps",  5000  ) # max_iter Nombre max d'iter de la methode iterative
Params_set_oneparam(MyParams, "matrix_solver_params","eps_steps", 1.e-13) # eps_iter arret des iterations "  "  "   "

Params_set_oneparam(MyParams, "matrix_solver_params","iter_info", ITER_INFOe_ALL )       # iter_info (0 , 1=stdout, 2=file iter_file)
Params_set_oneparam(MyParams, "matrix_solver_params","iter_file", "the_residuals.dat"  ) # iter_file le nom du fichier

Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_MESCHACH) # USE BAND WIDTH REDUCTION BANDWRe_BAND
Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_option", BANDWRe_BAND)   # USE BAND WIDTH REDUCTION


Params_set_staticparam(MyParams, 0)


MyElt  = elt2D_get(Params_get_oneparam(MyParams, "finite_elements_params", "name_ef"))

elt_P1 = elt2D_get("P1")
elt_P2 = elt2D_get("P2")
elt_P3 = elt2D_get("P3")

# --------------------------------------------------------------------

MyGeom = Geom2D_get(MyElt,
        Params_get_oneparam(MyParams, "geometry_params", "meshfile"),
        Params_get_oneparam(MyParams, "geometry_params", "meshname"),
        Params_get_oneparam(MyParams, "geometry_params", "meshtype"))

Geom2D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X)  
Geom2D_foutput(fp2, MyGeom)  

# --------------------------------------------------------------------
# --------------------------------------------------------------------
def Py_solve2D_laplacian( MyElt , MyGeom , MyBC , MyRhsFun ) :

    NBSOMM = GEOM_2D_NBSOMM_get(MyGeom)

    ResolutionMethod = Params_get_oneparam(MyParams,"matrix_solver_params","resolution_method")
    preconditionning = Params_get_oneparam(MyParams,"matrix_solver_params","preconditionning")
    print("ResolutionMethod =", ResolutionMethod)
    print("Preconditionning =", preconditionning)

    eps_steps = Params_get_oneparam(MyParams,"matrix_solver_params","eps_steps")
    max_steps = Params_get_oneparam(MyParams,"matrix_solver_params","max_steps")
    nb_steps  = new_intp()

    A = sp_get(NBSOMM,NBSOMM,10)
    A_stiff = sp_get(NBSOMM,NBSOMM,10)
    A_bc = sp_get(NBSOMM,NBSOMM,10)

    RHS = v_get(NBSOMM)
    RHS_fun = v_get(NBSOMM)
    RHS_bc = v_get(NBSOMM)
    
    VIT = v_get(NBSOMM)

    # ----- assemblage matrix and rhs ------ 

    A_stiff = assemblage2D_matrix_Stiff1( MyElt , MyGeom , A_stiff )
    A_bc = assemblage2D_matrix_fromBC( MyElt , MyGeom , MyBC, A_bc )

    RHS_fun = assemblage2D_vector_fun( MyElt , MyGeom , MyRhsFun, RHS_fun )
    RHS_bc = assemblage2D_vector_fromBC( MyElt , MyGeom , MyBC, RHS_bc )

    AA = sp_add(A_stiff, A_bc, A)
    RHS = v_add(RHS_fun, RHS_bc, RHS)
    
    transform2D_matrix_vector_with_bc( MyElt , MyGeom , MyBC , A , RHS )


    graphics_matrix_pattern("gnuplot", A, "matrix_profile") # no empty character !

    # ------ solve the system Ax = b   ----- 

    ILU = sp_null()
    ICH = sp_null()

    if  ResolutionMethod == "DIRECT-METHOD" :

        BANDWR_METHOD = Params_get_oneparam(MyParams, "matrix_solver_params", "bandwidth_method")

        t1 = time.time()

        if   ( BANDWR_METHOD == BANDWRe_AMD ):
            spCHresolution_amd(A, RHS, VIT)
        elif ( BANDWR_METHOD == BANDWRe_SPOOLES ):    
            spCHresolution_spooles(A, RHS, VIT)
        elif ( BANDWR_METHOD == BANDWRe_MESCHACH ): 
            spCHresolution_meschach(A, RHS, VIT)
        else :
            spCHresolution_meschach(A, RHS, VIT)


        t2 = time.time()

        print("\nt2-t1 = ", t2-t1)

        SP_FREE(A)
        V_FREE(RHS)

        return VIT

    # other methods "ITERATIVES"...
    
    if preconditionning == "ICH" :

        ICH = sp_copy(A)
        spICHfactor(ICH)    

    elif preconditionning == "ILU" :

        print("for problem = \"Laplacien\", the matrix A should be symmetric")
        print(" -> use preferrably the LLT preconditionning + CG ")

        ILU = sp_copy(A)
        spILUfactor(ILU,0.0)    

    elif preconditionning == "DIAG" :

        print("not implemented -> NULL precond")

    elif preconditionning == "NULL" :

        print("no precond for iterativ method")

    else :

        warnings.warn(WARNINGS_DICT["W_PRECONDITIONNING"])



    if  ResolutionMethod == "CG" :

        iter_xspcg(A,ICH,RHS,eps_steps,VIT,max_steps,nb_steps,None)
        print("cg: # of iter. = ", intp_value(nb_steps))

    elif ResolutionMethod == "BiCGStab" :

        iter_xspbicgstab(A,ILU,RHS,eps_steps,VIT,max_steps,nb_steps,None)
        print("bicgstab: # of iter. = ", intp_value(nb_steps))

    else :

        raise AssertionError, ERRORS_DICT["E_METHOD"]



    #delete_int_p(nb_steps)

    V_FREE(RHS)
    SP_FREE(A)

    if preconditionning == "ILU" :
        SP_FREE(ILU)
    if preconditionning == "ICH" or ResolutionMethod == "DIRECT-METHOD" :
        SP_FREE(ICH)


    return VIT
 

# --------------------------------------------------------------------
# --------------------------------------------------------------------

PROBLEM = Params_get_oneparam(MyParams, "main_problem", "NULL")
print("PROBLEM is ", PROBLEM)


if   PROBLEM == "HELMHOLZ" :

    (eigen_vectors, eigen_values) = Py_solve2D_eigen( MyElt , MyGeom , MyBC )

elif PROBLEM == "LAPLACIAN" :

    SOL = Py_solve2D_laplacian( MyElt , MyGeom , MyBC , MyRhsFun )    

elif PROBLEM == "CONVECTION-DIFFUSION" :

    SOL = solve2D_convection_diffusion( MyElt , MyGeom , MyBC , MyRhsFun , MyAdvFun )

elif PROBLEM == "STOKES" :

    (U,V,P) = Py_solve2D_stokes( elt_P2 , elt_P1 , MyGeom , MyBC , MyRhsFun )

elif PROBLEM == "NAVIER-STOKES" :

    (U,V,P) = Py_solve2D_navier_stokes( elt_P2 , elt_P1 , MyGeom , MyBC , MyRhsFun )

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

    #graphics2D_vogle_plotsurface(VOGLEe_COLOR_RED, SOL, GEOM_2D_NSELMT_get(MyGeom), GEOM_2D_XYSOMM_get(MyGeom) )
    
    print("XXX", VOPL_CONTOURS)
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

    graphics2D_vogle_plotsurface(VOGLEe_COLOR_RED, SOL, GEOM_2D_NSELMT_get(MyGeom), GEOM_2D_XYSOMM_get(MyGeom) )

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
v_foutput(fp1, SOL)
fp1.close()

fp2 = open("xysomm_2.dat", "w")
m_foutput(fp2, GEOM_2D_XYSOMM_get(MyGeom))
fp2.close()

fp3 = open("nselmt_2.dat", "w")
im_foutput(fp3, GEOM_2D_NSELMT_get(MyGeom))
fp3.close()

#---------------------------------------------------------------------

# free memory
# -----------

ELT_2D_FREE(MyElt)

ELT_2D_FREE(elt_P1)
ELT_2D_FREE(elt_P2)
ELT_2D_FREE(elt_P3)

GEOM_2D_FREE(MyGeom)
BC_2D_FREE(MyBC)
RHS_2D_FREE(MyRhsFun)

PARAMS_FREE(MyParams)

#---------------------------------------------------------------------

#mem_info_file(sys.stdout,0)
# 
#mem_info_file(sys.stdout,MY_LIST1)
#mem_info_file(sys.stdout,MY_LIST2) 
#mem_info_file(sys.stdout,MY_LIST3) 
#mem_info_file(sys.stdout,MY_LIST4) 
#mem_info_file(sys.stdout,MY_LIST5) 
#mem_info_file(sys.stdout,MY_LIST6)
#mem_info_file(sys.stdout,MY_LIST7) 

fp2.close()

#----------------------------------------------------------------------

Py_pthread_exit()

