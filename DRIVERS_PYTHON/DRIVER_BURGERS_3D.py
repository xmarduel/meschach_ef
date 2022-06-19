#! /usr/bin/env python

import sys
import math

from meschach import *
from meschach_spooles import *
from meschach_adds import *
from meschach_ef import *
from meschach_cpgplot import *

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

set_err_flag(EF_JUMP)
set_warn_flag(EF_SILENT)

#-------------------------------------------------------------------

#def spheric_src(x,y,z):
#    return ( - spheric_xx(x,y,z) - spheric_yy(x,y,z) - spheric_zz(x,y,z) + spheric(x,y,z)*spheric_x(x,y,z) + #spheric(x,y,z)*spheric_y(x,y,z) + spheric(x,y,z)*spheric_z(x,y,z))
def spheric_src(x,y,z):
    return ( - spheric_xx(x,y,z) - spheric_yy(x,y,z) - spheric_zz(x,y,z) + spheric(x,y,z)*spheric(x,y,z) )

def spheric(x,y,z):
    #return 10.0/4.0*(0.5 - (x-0.5)*(x-0.5) - (y-0.5)*(y-0.5) - (z-0.5)*(z-0.5) )
    return 5*x + 2*y + 4*z
def spheric_x(x,y,z):
    #return 10.0/4.0*( - 2*(x-0.5) )
    return 5.0
def spheric_y(x,y,z):
    #return 10.0/4.0*( - 2*(y-0.5) )
    return 2.0
def spheric_z(x,y,z):
    #return 10.0/4.0*( - 2*(z-0.5) )
    return 4.0
def spheric_xx(x,y,z):
    #return -10.0/2.0
    return 0.0
def spheric_yy(x,y,z):
    #return -10.0/2.0
    return 0.0
def spheric_zz(x,y,z):
    #return -10.0/2.0
    return 0.0

def bc0(x,y,z):
    return 0.0



MyBC = Bc3D_get()

Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 1, AXEe_X )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 2, AXEe_X )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 3, AXEe_X )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 4, AXEe_X )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 5, AXEe_X )
Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 6, AXEe_X )

Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 1, AXEe_X, spheric )
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 2, AXEe_X, spheric )
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 3, AXEe_X, spheric )
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 4, AXEe_X, spheric )
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 5, AXEe_X, spheric )
Bc3D_setFunctionPython(MyBC, BC_3De_DIRICHLET, 6, AXEe_X, spheric )

Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 1, AXEe_X, spheric)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 2, AXEe_X, spheric)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 3, AXEe_X, spheric)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 4, AXEe_X, spheric)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 5, AXEe_X, spheric)
Bc3D_setFunctionPython(MyBC, BC_3De_NEUMANN, 6, AXEe_X, spheric)


MyRhsFun = Rhs3D_get()
Rhs3D_setFunctionPython(MyRhsFun, 0, AXEe_X, spheric_src) # ref_e=0, axe=1
Rhs3D_setFunctionPython(MyRhsFun, 0, AXEe_Y, spheric_src) # ref_e=0, axe=2
Rhs3D_setFunctionPython(MyRhsFun, 0, AXEe_Z, spheric_src) # ref_e=0, axe=3


MyAdv = Adv3D_get()
MyAdv = Adv3D_setFunctionPython( MyAdv, 0, AXEe_X, AXEe_X, AXEe_Z, spheric_z)
MyAdv = Adv3D_setFunctionPython( MyAdv, 0, AXEe_X, AXEe_X, AXEe_Y, spheric_z)
MyAdv = Adv3D_setFunctionPython( MyAdv, 0, AXEe_X, AXEe_X, AXEe_Z, spheric_z)
MyAdv = Adv3D_setFunctionPython( MyAdv, 0, AXEe_Y, AXEe_Z, AXEe_X, spheric_z)


a = Adv3D_evalFunction(MyAdv, 0, AXEe_X, AXEe_X, AXEe_Z,1,1,1)
a = Adv3D_evalFunction(MyAdv, 0, AXEe_X, AXEe_X, AXEe_Y,1,1,1)
a = Adv3D_evalFunction(MyAdv, 0, AXEe_X, AXEe_X, AXEe_Z,1,1,1)
a = Adv3D_evalFunction(MyAdv, 0, AXEe_Y, AXEe_Z, AXEe_X,1,1,1)
a = Adv3D_evalFunction(MyAdv, 0, AXEe_Y, AXEe_Y, AXEe_Z,1,1,1)


#-------------------------------------------------------------------

MyParams = Params_get()
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "main_problem","NULL", "Burgers" )  # Laplacian
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "finite_elements_params","name_ef", "P2" )    # Type d'EF : "P1","P2"

Params_set_oneparam(MyParams, "matrix_solver_params","resolution_method", "DIRECT-METHOD" )     # Methode : DIRECT-METHOD,CG,CGS
Params_set_oneparam(MyParams, "matrix_solver_params","preconditionning", "NULL" )   # Precond : NULL, ICH, ILU

Params_set_oneparam(MyParams, "geometry_params","meshfile", "cube3D_P1.cube" )  #  Mesh File ("name.dat") "MESH_P1.quad2"
                                                                                            "cube2D_P1_v3_ok2.gmsh"

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
Params_set_oneparam(MyParams, "matrix_solver_params","eps_steps", 1.e-13) # eps_iter arret des iterations "  "  "   "

Params_set_oneparam(MyParams, "miscellaneous_params","iter_info", ITER_INFOe_ALL )       # iter_info (0 , 1=stdout, 2=file iter_file)
Params_set_oneparam(MyParams, "miscellaneous_params","iter_file", "the_residuals.dat"  ) # iter_file le nom du fichier

Params_set_oneparam(MyParams, "matrix_solver_params","bandwidth_method", BANDWRe_AMD)       # USE BAND WIDTH REDUCTION


Params_set_staticparam(MyParams, 0)


MyElt  = elt3D_get(Params_get_oneparam(MyParams, "finite_elements_params", "name_ef"))

elt_P2 = elt3D_get("P2")
elt_P1 = elt3D_get("P1")

#--------------------------------------------------------------------

MyGeom = Geom3D_get(MyElt, Params_get_oneparam(MyParams, "geometry_params", "meshfile"))

Geom3D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X)  

# --------------------------------------------------------------------------------------------
def Py_solve3D_burgers( MyElt , MyGeom , MyBC , MyRhsFun ) :
   
    NBSOMM = GEOM_3D_NBSOMM_get(MyGeom)
    
    print("NBSOMM = ", NBSOMM)
    #
    ResolutionMethod = Params_get_oneparam(MyParams,"matrix_solver_params","resolution_method")
    Preconditionning = Params_get_oneparam(MyParams,"matrix_solver_params","preconditionning")
    print("ResolutionMethod =", ResolutionMethod)
    print("Preconditionning =", Preconditionning)
    
    eps_steps = Params_get_oneparam(MyParams,"matrix_solver_params","eps_steps")
    max_steps = Params_get_oneparam(MyParams,"matrix_solver_params","max_steps")
    nb_steps  = new_intp()
    
    A   = sp_get(NBSOMM,NBSOMM,10)
    Abc = sp_get(NBSOMM,NBSOMM,10)
    
    SOL = v_get(NBSOMM); v_zero(SOL)
    
    Un  = v_get(NBSOMM); v_zero(Un)
    
    UU  = v_get(NBSOMM)
    UUx = v_get(NBSOMM)
    UUy = v_get(NBSOMM)
    UUz = v_get(NBSOMM)
    LIN = v_get(NBSOMM)
    RHS = v_get(NBSOMM)
    
    # ----- assemblage matrix and rhs ------ 
    
    A   = assemblage3D_matrix_Stiff1( MyElt , MyGeom , A )
    Abc = sp_copy2(A, Abc)
    
    # calculate once for all
    LIN  = assemblage3D_vector_fun( MyElt , MyGeom , MyRhsFun , LIN )
    
    transform3D_matrix_with_bc( MyElt , MyGeom , MyBC , Abc )
    
    # ------ solve the system Ax = b   ----- 
    
    ILU = sp_null()
    ICH = sp_null()
    
    Pe    = px_get(NBSOMM)
    INVPe = px_get(NBSOMM)
    
    if  ResolutionMethod == "DIRECT-METHOD" :
        #
        ICH = sp_copy(Abc)
        ICH = sp_transformwithbandwr(Abc , Pe, INVPe, BANDWRe_AMD, BANDWRe_AMD, ICH )
        
        spCHfactor(ICH)
    
    
    elif Preconditionning == "ICH" :
       
        ICH = sp_copy(Abc)
        spICHfactor(ICH)    
    
    elif Preconditionning == "ILU" :
        #
        print("for problem = \"Laplacien\", the matrix A should be symmetric")
        print("-> use preferrably the LLT preconditionning + CG ")
        
        ILU = sp_copy(Abc)
        spILUfactor(ILU,0.0)    

    elif Preconditionning == "NULL" :
       
        print("no precond for iterativ method")

    else :
       
        warnings.warn(WARNINGS_DICT["W_PRECONDITIONNING"])




    for n in range(300):
    
        v_copy( SOL, Un )
        
        UU   = assemblage3D_vector_ab ( MyElt , MyGeom , Un, Un , UU  )   #  UU    = [ U_n * U_n       ]
        #UUx  = assemblage3D_vector_abx( MyElt , MyGeom , Un, Un , UUx )   #  UUx   = [ U_n * DX( U_n ) ]
        #UUy  = assemblage3D_vector_aby( MyElt , MyGeom , Un, Un , UUy )   #  UUy   = [ U_n * DY( U_n ) ]
        #UUz  = assemblage3D_vector_abz( MyElt , MyGeom , Un, Un , UUz )   #  UUz   = [ U_n * DZ( U_n ) ]
        
        RHS = v_copy(LIN, RHS)
        RHS = v_sub( RHS, UU, RHS)
        #RHS = v_sub( RHS, UUx, RHS)
        #RHS = v_sub( RHS, UUy, RHS)
        #RHS = v_sub( RHS, UUz, RHS)
        
        transform3D_vector_with_bc( MyElt , MyGeom , MyBC , A, RHS )
        
        
        if  ResolutionMethod == "DIRECT-METHOD" :

            #spCHsolve(ICH, RHS, SOL)
            spCHsolve_bandwr(ICH, Pe, INVPe, RHS, SOL)  
        
        elif  ResolutionMethod == "CG" :
        
            iter_spcg2(A,ICH,RHS,eps_steps, SOL,max_steps,nb_steps)
            print("cg: # of iter. = ",intp_value(nb_steps))

        elif ResolutionMethod == "BiCGStab" :
        
            iter_spbicgstab(A,ILU,RHS,eps_steps, SOL,max_steps,nb_steps)
            print("bicgstab: # of iter. = ",intp_value(nb_steps))

        else :
        
            raise AssertionError(ERRORS_DICT["E_METHOD"])


        # test convergence
        diff = v_norm2( v_sub(SOL, Un, RHS) )
        print("iter=",n,"  -> diff = ", diff )

        if ( diff < eps_steps ): break
        
        

    delete_intp(nb_steps)
    
    V_FREE(RHS)
    SP_FREE(A) 
    if Preconditionning == "ILU" :
        SP_FREE(ILU)
    if Preconditionning == "ICH" or ResolutionMethod == "DIRECT-METHOD" :
        SP_FREE(ICH)

    return SOL

# --------------------------------------------------------------------
# --------------------------------------------------------------------

PROBLEM = Params_get_oneparam(MyParams, "main_problem", "NULL")
print("PROBLEM is ",PROBLEM)


if   PROBLEM == "LAPLACIAN" :

    SOL = solve3D_laplacien( MyElt , MyGeom , MyBC , MyRhsFun )    

elif PROBLEM == "CONVECTION-DIFFUSION" :

    SOL = solve3D_convection_diffusion( MyElt , MyGeom , MyBC , MyRhsFun )

elif PROBLEM == "BURGERS" :

    SOL = Py_solve3D_burgers( MyElt , MyGeom , MyBC , MyRhsFun )

elif PROBLEM == "STOKES" :

    (U,V,P) = Py_solve3D_stokes( MyElt , elt_P1 , MyGeom , MyBC , MyRhsFun )

elif PROBLEM == "NAVIER-STOKES" :

    #(U,V,P) = Py_solve3D_navier_stokes( elt_P2 , elt_P1 , MyGeom , MyBC , MyRhsFun )
	 pass
	
else:

	 print("Problem \"%s\" not yet implemented" % PROBLEM)
    sys.exit(EXIT_FAILURE)


#----------------------- graphics output -----------------------------

if   PROBLEM == "LAPLACIAN" :

	 graphics3D( "gnuplot" , MyElt , MyGeom , SOL , "Sol_Approch")
    #graphics3D( "vtk" , MyElt , MyGeom , SOL , "Sol_Approch")

elif PROBLEM == "CONVECTION-DIFFUSION" :

	 graphics3D( "vtk" , MyElt , MyGeom , SOL , "Sol_Approch")

elif PROBLEM == "BURGERS" :

	 graphics3D( "vtk" , MyElt , MyGeom , SOL , "Sol_Approch")

elif PROBLEM == "STOKES" :

	 #graphics3D( "vtk" , MyElt , MyGeom , U , "Sol_U_Approch")
	 #graphics3D( "vtk" , MyElt , MyGeom , V , "Sol_V_Approch")
	 #graphics3D( "vtk" , MyElt , MyGeom , P , "Sol_P_Approch")
	
    graphics3D_stokes( "vtk", MyElt , MyGeom , U, V, P, "Stokes")

elif PROBLEM == "NAVIER-STOKES" :

	 pass

else:

    print("Problem \"%s\" not yet implemented" % PROBLEM)
    sys.exit(EXIT_FAILURE)

#---------------------------------------------------------------------

if True :

    fun3D = Fun3D_get()
    Fun3D_setFunctionPython(fun3D, spheric) # ref_e=0, axe=1
    #Fun3D_setCFunction(fun3D, ExSol3D[MyParams->misc_params.Exact_sol] ) 

    diff = diff_vec_function3D( MyElt, MyGeom, SOL, fun3D, SOL )
    print("diff solexacte-solapprochee = %le" % diff)
      
    #v_foutput(sys.stdout, SOL)
    
    FUN_3D_FREE(fun3D)



# free memory
# -----------

ELT_3D_FREE(MyElt)

ELT_3D_FREE(elt_P1)
ELT_3D_FREE(elt_P2)

GEOM_3D_FREE(MyGeom)
BC_3D_FREE(MyBC)
RHS_3D_FREE(MyRhsFun)

#V_FREE(U)
#V_FREE(V)
#V_FREE(P)

PARAMS_FREE(MyParams)

#---------------------------------------------------------------------

#mem_info_file(sys.stdout,0)

#mem_info_file(sys.stdout,MY_LIST1)
#mem_info_file(sys.stdout,MY_LIST2) 
#mem_info_file(sys.stdout,MY_LIST3) 
#mem_info_file(sys.stdout,MY_LIST4) 
#mem_info_file(sys.stdout,MY_LIST5) 
#mem_info_file(sys.stdout,MY_LIST6)
#mem_info_file(sys.stdout,MY_LIST7) 

#----------------------------------------------------------------------
