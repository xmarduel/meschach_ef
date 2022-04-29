
from sys import *

from meschach          import *
from meschach_adds     import *
from meschach_ef       import *
from meschach_cpgplot  import *

from meschach_spooles  import *

# --------------------------------------------------------------------------------------------

def Py_solve2D_burgers1(MyElt, MyGeom, MyBC, MyRhsFun, MyParams):
    """
    solve A*Un = F - UUx - UUy - UU
    
       with A = STIFF
            U = Unm1 
    """
    NBSOMM = GEOM_2D_NBSOMM_get(MyGeom)

    ResolutionMethod = Params_get_oneparam(MyParams,"matrix_solver_params","resolution_method")
    preconditionning = Params_get_oneparam(MyParams,"matrix_solver_params","preconditionning")
    print "ResolutionMethod =", ResolutionMethod
    print "Preconditionning =", preconditionning

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
    LIN = v_get(NBSOMM)
    RHS = v_get(NBSOMM)

    # ---- assemblage matrix and rhs ------

    A   = assemblage2D_matrix_Stiff1( MyElt , MyGeom , A )
    Abc = sp_copy2(A, Abc)

    # calculate once for all
    LIN  = assemblage2D_vector_fun( MyElt , MyGeom , MyRhsFun , LIN )

    transform2D_matrix_with_bc( MyElt , MyGeom , MyBC , Abc )

    # ------ solve the system Ax = b   ----- 

    ILU = sp_null()
    ICH = sp_null()

    Pe  = px_get(NBSOMM)
    INVPe = px_get(NBSOMM)
    
    if  ResolutionMethod == "DIRECT-METHOD" :

        ICH = sp_copy(Abc)
        ICH = sp_transformwithbandwr(Abc , Pe, INVPe, BANDWRe_AMD, 0, ICH )

        spCHfactor(ICH)


    elif preconditionning == "ICH" :

        ICH = sp_copy(Abc)
        spICHfactor(ICH)    

    elif preconditionning == "ILU" :

        print "for problem = \"Laplacien\", the matrix A should be symmetric"
        print" -> use preferrably the LLT preconditionning + CG "

        ILU = sp_copy(Abc)
        spILUfactor(ILU,0.0)    

    elif preconditionning == "DIAG" :

        print "not implemented -> NULL precond"

    elif preconditionning == "NULL" :

        print "no precond for iterativ method"

    else :

        warnings.warn(WARNINGS_DICT["W_PRECONDITIONNING"])

    # --------------------------------------------------------------------

    for n in range(100):

        v_copy( SOL, Un )


        UU  = assemblage2D_vector_ab ( MyElt , MyGeom , Un, Un , UU  )    #  UU    = [ U_n * U_n ]

        UUx  = assemblage2D_vector_abx( MyElt , MyGeom , Un, Un , UUx )   #  UUx   = [ U_n * DX( U_n ) ]
        UUy  = assemblage2D_vector_aby( MyElt , MyGeom , Un, Un , UUy )   #  UUx   = [ U_n * DY( U_n ) ]

        RHS = v_copy(LIN, RHS)
        RHS = v_sub( RHS, UU, RHS)
        RHS = v_sub( RHS, UUx, RHS)
        RHS = v_sub( RHS, UUy, RHS)

        transform2D_vector_with_bc( MyElt , MyGeom , MyBC , A, RHS )


        if  ResolutionMethod == "DIRECT-METHOD" :

            #spCHsolve(ICH, RHS, SOL)
            spCHsolve_bandwr(ICH, Pe, INVPe, RHS, SOL)  

        elif  ResolutionMethod == "CG" :

            iter_spcg2(A,ICH,RHS,eps_steps, SOL,max_steps,nb_steps)
            print "cg: # of iter. = ",intp_value(nb_steps)

        elif ResolutionMethod == "BiCGStab" :

            iter_spbicgstab(A,ILU,RHS,eps_steps, SOL,max_steps,nb_steps)
            print "bicgstab: # of iter. = ",intp_value(nb_steps)

        else :

            raise AssertionError, ERRORS_DICT["E_METHOD"]


        # test convergence
        diff = v_norm2( v_sub(SOL, Un, RHS) )
        print "iter=",n,"  -> diff = ", diff 

        if diff < eps_steps:
            break




         
    delete_intp(nb_steps)

    V_FREE(RHS)
    SP_FREE(A) 
    if preconditionning == "ILU" :
        SP_FREE(ILU)
    if preconditionning == "ICH" or ResolutionMethod == "DIRECT-METHOD" :
        SP_FREE(ICH)


    return SOL

#
# ---------------------------------------------------------------------------------------
#
def Py_solve2D_burgers2( MyElt , MyGeom , MyBC , MyRhsFun , MyParams ) :

    """
    solve A*Un = F 
    
       with A = STIFF + Cx(U) + Cy(U) + D(U)
            U = Unm1
            
            Cx(U) from term U_Ux 
            Cy(U) from term U_Uy 
            D(U)  from term U_U 
    """
    NBSOMM = GEOM_2D_NBSOMM_get(MyGeom)

    ResolutionMethod = Params_get_oneparam(MyParams,"matrix_solver_params","resolution_method")
    preconditionning = Params_get_oneparam(MyParams,"matrix_solver_params","preconditionning")
    print "ResolutionMethod =", ResolutionMethod
    print "preconditionning =", preconditionning

    eps_steps = Params_get_oneparam(MyParams,"matrix_solver_params","eps_steps")
    max_steps = Params_get_oneparam(MyParams,"matrix_solver_params","max_steps")
    nb_steps  = new_intp()

    S   = sp_get(NBSOMM,NBSOMM,10)
    Cx  = sp_get(NBSOMM,NBSOMM,10)
    Cy  = sp_get(NBSOMM,NBSOMM,10)
    D   = sp_get(NBSOMM,NBSOMM,10)

    A   = sp_get(NBSOMM,NBSOMM,10)
    Abc = sp_get(NBSOMM,NBSOMM,10)

    RES = v_get(NBSOMM)
    SOL = v_get(NBSOMM)

    Un   = v_get(NBSOMM) v_zero(Un)

    RHS   = v_get(NBSOMM)
    RHSbc = v_get(NBSOMM)

    # ----- assemblage matrix and rhs ------

    S    = assemblage2D_matrix_Stiff1( MyElt , MyGeom , S )

    RHS  = assemblage2D_vector_fun   ( MyElt , MyGeom , MyRhsFun , RHS )


    for n in range(100):

        v_copy( SOL, Un )

        v_copy(RHS, RHSbc)


        Cx  = assemblage2D_matrix_AUx   ( MyElt , MyGeom , Un, Cx ) 
        Cy  = assemblage2D_matrix_AUy   ( MyElt , MyGeom , Un, Cy )
        D   = assemblage2D_matrix_A_U   ( MyElt , MyGeom , Un, D  )


        sp_add(S, Cx, A)
        sp_add(A, Cy, A)
        sp_add(A, D , A)

        Bbc = sp_copy2(A, Abc)

        transform2D_matrix_vector_with_bc( MyElt , MyGeom , MyBC , Abc , RHSbc )


        if  ResolutionMethod == "DIRECT-METHOD" :

            #spLUfactor_solve_bridge_meschach(Abc, RHSbc, SOL)  
            spLUfactor_solve_bridge_amd(Abc, RHSbc, SOL)
            #spLUfactor_solve_bridge_spooles(Abc, RHSbc, SOL)  

        elif ResolutionMethod == "BiCGStab" :

            iter_spbicgstab(Abc,ILU, RHSbc,eps_steps, SOL,max_steps,nb_steps)
            print "bicgstab: # of iter. = ",int_p_value(nb_steps)

        else :

            raise AssertionError, ERRORS_DICT["E_METHOD"]
        

        # test convergence
        diff = v_norm2( v_sub(SOL, Un, RES) )
        print "iter=", n,"  -> diff = ", diff

        if diff < eps_steps:
            break

        #test residu
        res = v_norm2( v_sub( sp_mv_mlt(Abc, Un, RES), RHSbc, RES) )
        print "\niter=",n,"  -> res = ", res

         
    delete_intp(nb_steps)

    V_FREE(RHS); V_FREE(RHSbc)
    V_FREE(RES)
    V_FREE(Un)

    SP_FREE(A); SP_FREE(Abc) 

    SP_FREE(S) 
    SP_FREE(Cx) 
    SP_FREE(Cy) 
    SP_FREE(D) 

    return SOL

