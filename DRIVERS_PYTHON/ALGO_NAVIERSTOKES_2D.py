

from sys import *

from meschach          import *
from meschach_adds     import *
from meschach_ef       import *
from meschach_spooles  import *


# --------------------------------------------------------------------------------------------
def Py_solve2D_NavierStokes( MyElt , MyEltM1, MyGeom , MyBC , MyRhsFun ) :
    """
    solve Navier-Stokes
    
    2 methods : 1-NEWTON-EXACT
                2-NEWTON-APPROCH
    """

    MyParams = Params_get_staticparam(0)
	 
    METHOD = Params_get_oneparam(MyParams, "navierstokes_params", "method")


    if METHOD == "NEWTON-EXACT" :

        return Py_solve2D_NavierStokes_NEWTON_EXACT( MyElt , MyEltM1, MyGeom , MyBC , MyRhsFun , MyParams )

    elif METHOD == "NEWTON-APPROCH" :

        return Py_solve2D_NavierStokes_NEWTON_APPROCH( MyElt , MyEltM1, MyGeom , MyBC , MyRhsFun , MyParams )

    else:

        print "METHOD = %s" % METHOD
        raise  TypeError, "???????? Wrong Method ???????"


# --------------------------------------------------------------------------------------------
def Py_solve2D_NavierStokes_NEWTON_EXACT( MyElt , MyEltM1, MyGeom , MyBC , MyRhsFun , MyParams ) :
    """
    solve Navier-Stokes - NEWTON-EXACT Algorithm
    
    (U,V,P) = Py_solve2D_NavierStokes_NE( MyElt , MyEltM1, MyGeom , MyBC , MyRhsFun , MyParams )
    """
	 
    NBSOMM_U = GEOM_2D_NBSOMM_get(MyGeom)
    NBSOMM_P = GEOM_2D_NBSOMM_get(GEOM_2D_geomBase_get(MyGeom))

    eps_steps = Params_get_oneparam(MyParams, "navierstokes_params", "eps_steps")
    max_steps = Params_get_oneparam(MyParams, "navierstokes_params", "max_steps")
    nb_steps  = new_intp()

    kappa     = Params_get_oneparam(MyParams, "physical_params", "kappa")
    Reynolds  = 1.0 / kappa
    print "Reynolds =", Reynolds


    STOKES        = sp_get(2*NBSOMM_U + NBSOMM_P, 2*NBSOMM_U + NBSOMM_P, 10)
    STOKES_RHS    =  v_get(2*NBSOMM_U + NBSOMM_P)

    STOKES_bc     = sp_get(2*NBSOMM_U + NBSOMM_P, 2*NBSOMM_U + NBSOMM_P, 10)
    STOKES_RHS_bc =  v_get(2*NBSOMM_U + NBSOMM_P)

    STOKES_PRECOND = sp_get(2*NBSOMM_U + NBSOMM_P, 2*NBSOMM_U + NBSOMM_P, 10)
    EYE_P          = sp_get(  NBSOMM_P           , NBSOMM_P,2)

    sp_eye(EYE_P)

    Su     = sp_get(NBSOMM_U, NBSOMM_U,10) # stiff

    UnUx    = sp_get(NBSOMM_U, NBSOMM_U,10)
    VnUy    = sp_get(NBSOMM_U, NBSOMM_U,10)
    UnVx    = sp_get(NBSOMM_U, NBSOMM_U,10)
    VnVy    = sp_get(NBSOMM_U, NBSOMM_U,10)

    UUnx    = sp_get(NBSOMM_U, NBSOMM_U,10)
    VUny    = sp_get(NBSOMM_U, NBSOMM_U,10)
    UVnx    = sp_get(NBSOMM_U, NBSOMM_U,10)
    VVny    = sp_get(NBSOMM_U, NBSOMM_U,10)

    UnUx_p_VnUy_p_UUnx = sp_get(NBSOMM_U, NBSOMM_U,10)
    UnVx_p_VnVy_p_VVny = sp_get(NBSOMM_U, NBSOMM_U,10)

    Bx     = sp_get(NBSOMM_P, NBSOMM_U,10)
    By     = sp_get(NBSOMM_P, NBSOMM_U,10)

    BTx    = sp_get(NBSOMM_U, NBSOMM_P,10)
    BTy    = sp_get(NBSOMM_U, NBSOMM_P,10)

    J      = sp_get(NBSOMM_P, NBSOMM_P,10)
    C      = sp_get(NBSOMM_P, NBSOMM_P,10)


    UnUnx    = v_get(NBSOMM_U)
    VnUny    = v_get(NBSOMM_U)
    UnVnx    = v_get(NBSOMM_U)
    VnVny    = v_get(NBSOMM_U)

    RHS_NLIN_U  =  v_get(NBSOMM_U)
    RHS_NLIN_V  =  v_get(NBSOMM_U)

    STOKES_RHS_NLIN  =  v_get(2*NBSOMM_U + NBSOMM_P)

    Un  = v_get(NBSOMM_U); v_zero(Un)
    Vn  = v_get(NBSOMM_U); v_zero(Vn)

    Unp1 = v_get(NBSOMM_U)
    Vnp1 = v_get(NBSOMM_U)

    VIT = v_get(2*NBSOMM_U);   v_zero(VIT) # [ U , V ]
    P   = v_get(  NBSOMM_P);

    SOL = v_get(2*NBSOMM_U+ NBSOMM_P);   v_zero(SOL); # [ U , V , P])

	
    # ----- assemblage matrix and rhs ------ 

    J  = assemblage2D_matrix_Mass  ( MyEltM1 , GEOM_2D_geomBase_get(MyGeom) , J  )

    Su   = assemblage2D_matrix_Stiff1( MyElt , MyGeom , Su )
    sp_smlt(Su, kappa, Su)


    Bx = assemblage2D_matrix_Conv_x_elt2elt1( MyElt , MyGeom , MyEltM1 , GEOM_2D_geomBase_get(MyGeom) , Bx )
    By = assemblage2D_matrix_Conv_y_elt2elt1( MyElt , MyGeom , MyEltM1 , GEOM_2D_geomBase_get(MyGeom) , By )

    sp_smlt( Bx, -1.0, Bx)
    sp_smlt( By, -1.0, By)

    sp_transp(Bx, BTx)
    sp_transp(By, BTy)


    # rhs
    Rhs2D_setCurrentSelectedAxe(MyRhsFun, AXEe_X)
    Fu = v_get(NBSOMM_U)
    Fu = assemblage2D_vector_fun( MyElt, MyGeom, MyRhsFun, Fu)

    Rhs2D_setCurrentSelectedAxe(MyRhsFun, AXEe_Y)
    Fv = v_get(NBSOMM_U)
    Fv = assemblage2D_vector_fun( MyElt, MyGeom, MyRhsFun, Fv)


    STOKES_RHS = v_move(Fu, 0, NBSOMM_U, STOKES_RHS,        0) # set Fu into STOKES_RHS
    STOKES_RHS = v_move(Fv, 0, NBSOMM_U, STOKES_RHS, NBSOMM_U) # set Fv into STOKES_RHS


    A  = sp_get(2*NBSOMM_U, 2*NBSOMM_U, 10); sp_zero(A)
    B  = sp_get(  NBSOMM_P, 2*NBSOMM_U, 10); sp_zero(B)

    F   = v_get(2*NBSOMM_U)
    G   = v_get(  NBSOMM_P)


    v_zero(VIT)

    for n in range(max_steps):

        # get back velocity from the vector VIT
        Un = v_move(VIT,        0,NBSOMM_U, Un,0)
        Vn = v_move(VIT, NBSOMM_U,NBSOMM_U, Vn,0)


        UnUx  = assemblage2D_matrix_AUx   ( MyElt , MyGeom , Un, UnUx ) 
        VnUy  = assemblage2D_matrix_AUy   ( MyElt , MyGeom , Vn, VnUy )

        UnVx  = assemblage2D_matrix_AUx   ( MyElt , MyGeom , Un, UnVx ) 
        VnVy  = assemblage2D_matrix_AUy   ( MyElt , MyGeom , Vn, VnVy )


        UUnx  = assemblage2D_matrix_AxU   ( MyElt , MyGeom , Un, UUnx )
        VUny  = assemblage2D_matrix_AyU   ( MyElt , MyGeom , Un, VUny )

        UVnx  = assemblage2D_matrix_AxU   ( MyElt , MyGeom , Vn, UVnx ) 
        VVny  = assemblage2D_matrix_AyU   ( MyElt , MyGeom , Vn, VVny )


        sp_zero(UnUx_p_VnUy_p_UUnx)
        sp_add(UnUx, UnUx_p_VnUy_p_UUnx, UnUx_p_VnUy_p_UUnx)
        sp_add(VnUy, UnUx_p_VnUy_p_UUnx, UnUx_p_VnUy_p_UUnx)
        sp_add(UUnx, UnUx_p_VnUy_p_UUnx, UnUx_p_VnUy_p_UUnx)

        sp_zero(UnVx_p_VnVy_p_VVny)
        sp_add(UnVx, UnVx_p_VnVy_p_VVny, UnVx_p_VnVy_p_VVny)
        sp_add(VnVy, UnVx_p_VnVy_p_VVny, UnVx_p_VnVy_p_VVny)
        sp_add(VVny, UnVx_p_VnVy_p_VVny, UnVx_p_VnVy_p_VVny)


        sp_zero(STOKES)
        # assemblage of the big matrix "STOKES"
        STOKES = sp_move(Su , 0,0, NBSOMM_U,NBSOMM_U, STOKES,          0,       0)
        STOKES = sp_move(Su , 0,0, NBSOMM_U,NBSOMM_U, STOKES,   NBSOMM_U,NBSOMM_U)

        STOKES = sp_madd(UnUx_p_VnUy_p_UUnx, 0,0, NBSOMM_U,NBSOMM_U, STOKES,          0,       0)
        STOKES = sp_madd(UnVx_p_VnVy_p_VVny, 0,0, NBSOMM_U,NBSOMM_U, STOKES,   NBSOMM_U,NBSOMM_U)


        STOKES = sp_move(VUny, 0,0, NBSOMM_U,NBSOMM_U, STOKES,        0, NBSOMM_U)
        STOKES = sp_move(UVnx, 0,0, NBSOMM_U,NBSOMM_U, STOKES, NBSOMM_U,        0)

        STOKES = sp_move(Bx , 0,0, NBSOMM_P,NBSOMM_U, STOKES, 2*NBSOMM_U,        0)
        STOKES = sp_move(By , 0,0, NBSOMM_P,NBSOMM_U, STOKES, 2*NBSOMM_U, NBSOMM_U)

        STOKES = sp_move(BTx, 0,0, NBSOMM_U,NBSOMM_P, STOKES, 0*NBSOMM_U, 2*NBSOMM_U)
        STOKES = sp_move(BTy, 0,0, NBSOMM_U,NBSOMM_P, STOKES, 1*NBSOMM_U, 2*NBSOMM_U)


        STOKES_bc     = sp_copy2(STOKES,     STOKES_bc)


        UnUnx    = assemblage2D_vector_abx( MyElt , MyGeom , Un , Un , UnUnx )
        VnUny    = assemblage2D_vector_aby( MyElt , MyGeom , Vn , Un , VnUny )
        UnVnx    = assemblage2D_vector_abx( MyElt , MyGeom , Un , Vn , UnVnx )
        VnVny    = assemblage2D_vector_aby( MyElt , MyGeom , Vn , Vn , VnVny )

        RHS_NLIN_U = v_add( UnUnx, VnUny, RHS_NLIN_U)
        RHS_NLIN_V = v_add( UnVnx, VnVny, RHS_NLIN_V)

        STOKES_RHS_NLIN = v_move(RHS_NLIN_U, 0, NBSOMM_U, STOKES_RHS_NLIN,        0) # set RHS_NLIN_U into STOKES_RHS_NLIN
        STOKES_RHS_NLIN = v_move(RHS_NLIN_V, 0, NBSOMM_U, STOKES_RHS_NLIN, NBSOMM_U) # set RHS_NLIN_V into STOKES_RHS_NLIN


        STOKES_RHS_bc =  v_add(STOKES_RHS, STOKES_RHS_NLIN , STOKES_RHS_bc)


        # build the matrix "NAVIER_STOKES"
        transform2D_navierstokes_matrix_vector_with_bc( MyElt, MyGeom, GEOM_2D_geomBase_get(MyGeom), MyBC, STOKES_bc, STOKES_RHS_bc);


        OUTER_METHOD = Params_get_oneparam(MyParams, "navierstokes_params", "outer_method")


        sp_zero(A)
        sp_zero(B)
        # get A, B, F, G
        A = sp_move(STOKES_bc,          0,0, 2*NBSOMM_U,2*NBSOMM_U, A, 0,0)
        B = sp_move(STOKES_bc, 2*NBSOMM_U,0,   NBSOMM_P,2*NBSOMM_U, B, 0,0)

        F = v_move(STOKES_RHS_bc,          0,2*NBSOMM_U, F, 0)
        G = v_move(STOKES_RHS_bc, 2*NBSOMM_U,  NBSOMM_P, G, 0)


        if OUTER_METHOD == "UZAWA" :

            navierstokes_resol_uzawa (A, B, C, J, VIT, P, F, G)
        
        elif OUTER_METHOD == "PRESSUREMATRIX" :

            navierstokes_resol_pressurematrix(A, B, VIT, P, F, G)

        else:

            print "outer method for NavierStokes is \"%s\" \n" % METHOD
            error(E_UNKNOWN, "Py_solve2D_NavierStokes_NE")


        # get back velocity from the vector VIT
        Unp1 = v_move(VIT,        0,NBSOMM_U, Unp1,0);
        Vnp1 = v_move(VIT, NBSOMM_U,NBSOMM_U, Vnp1,0);
        # test convergence
        diff = v_norm2( v_sub(Unp1, Un, Unp1) ) + v_norm2( v_sub(Vnp1, Vn, Vnp1) ) 
        
        print "iter = %d -> diff = %lf" % (n, diff) 

        if diff < 100*eps_steps : 
		      break


    # get back velocity from the vector VIT
    Un = v_move(VIT,          0,NBSOMM_U, Un,0)
    Vn = v_move(VIT,   NBSOMM_U,NBSOMM_U, Vn,0)

    # clean
    SP_FREE(STOKES)
    SP_FREE(STOKES_bc)
    SP_FREE(STOKES_PRECOND)
    SP_FREE(EYE_P)

    SP_FREE(Su)

    SP_FREE(UnUx); SP_FREE(VnUy); SP_FREE(UnVx); SP_FREE(VnVy)
    SP_FREE(UUnx); SP_FREE(VUny); SP_FREE(UVnx); SP_FREE(VVny)

    SP_FREE(UnUx_p_VnUy_p_UUnx)
    SP_FREE(UnVx_p_VnVy_p_VVny)

    SP_FREE(Bx)
    SP_FREE(By)

    SP_FREE(BTx)
    SP_FREE(BTy)

    SP_FREE(J)
    SP_FREE(C)

    SP_FREE(A)
    SP_FREE(B)


    V_FREE(STOKES_RHS)
    V_FREE(STOKES_RHS_bc)

    V_FREE(UnUnx); V_FREE(VnUny); V_FREE(UnVnx); V_FREE(VnVny)

    V_FREE(RHS_NLIN_U)
    V_FREE(RHS_NLIN_V)

    V_FREE(STOKES_RHS_NLIN)

    V_FREE(Unp1)
    V_FREE(Vnp1)

    V_FREE(Fu)
    V_FREE(Fv)

    V_FREE(VIT)
    V_FREE(F)
    V_FREE(G)

    return (Un,Vn,P)


# --------------------------------------------------------------------------------------------
def Py_solve2D_NavierStokes_NEWTON_APPROCH( MyElt , MyEltM1, MyGeom , MyBC , MyRhsFun , MyParams ) :
    """
    solve Navier-Stokes - NEWTON_APPROCH Algorithm
    
    (U,V,P) = Py_solve2D_NavierStokes_NA( MyElt , MyEltM1, MyGeom , MyBC , MyRhsFun , MyParams )
    """

    NBSOMM_U = GEOM_2D_NBSOMM_get(MyGeom)
    NBSOMM_P = GEOM_2D_NBSOMM_get(GEOM_2D_geomBase_get(MyGeom))

    eps_steps = Params_get_oneparam(MyParams, "navierstokes_params", "eps_steps")
    max_steps = Params_get_oneparam(MyParams, "navierstokes_params", "max_steps")
    nb_steps  = new_intp()

    kappa     = Params_get_oneparam(MyParams, "physical_params", "kappa")
    Reynolds  = 1.0 / kappa
    print "Reynolds =", Reynolds

    Params_set_oneparam(MyParams, "stokes_params", "method", Params_get_oneparam(MyParams, "navierstokes_params", "outer_method" ))

    Params_set_oneparam(MyParams, "stokes_params", "pressurematrix.innerloop_method",                  Params_get_oneparam(MyParams, "navierstokes_params", "pressurematrix.innerloop_method" ))
    Params_set_oneparam(MyParams, "stokes_params", "pressurematrix.iterative_solver.max_steps",        Params_get_oneparam(MyParams, "navierstokes_params", "pressurematrix.iterative_solver.max_steps" ))
    Params_set_oneparam(MyParams, "stokes_params", "pressurematrix.iterative_solver.eps_steps",        Params_get_oneparam(MyParams, "navierstokes_params", "pressurematrix.iterative_solver.eps_steps" ))
    Params_set_oneparam(MyParams, "stokes_params", "pressurematrix.iterative_solver.bandwidth_option", Params_get_oneparam(MyParams, "navierstokes_params", "pressurematrix.iterative_solver.bandwidth_option" ))
    Params_set_oneparam(MyParams, "stokes_params", "pressurematrix.iterative_solver.bandwidth_method", Params_get_oneparam(MyParams, "navierstokes_params", "pressurematrix.iterative_solver.bandwidth_method" ))

    Params_set_oneparam(MyParams, "stokes_params", "uzawa.rho",                                        Params_get_oneparam(MyParams, "navierstokes_params", "uzawa.rho" ))
    Params_set_oneparam(MyParams, "stokes_params", "uzawa.iterative_solver.max_steps",                 Params_get_oneparam(MyParams, "navierstokes_params", "uzawa.iterative_solver.max_steps" ))
    Params_set_oneparam(MyParams, "stokes_params", "uzawa.iterative_solver.eps_steps",                 Params_get_oneparam(MyParams, "navierstokes_params", "uzawa.iterative_solver.eps_steps" ))
    Params_set_oneparam(MyParams, "stokes_params", "uzawa.iterative_solver.bandwidth_option",          Params_get_oneparam(MyParams, "navierstokes_params", "uzawa.iterative_solver.bandwidth_option" ))
    Params_set_oneparam(MyParams, "stokes_params", "uzawa.iterative_solver.bandwidth_method",          Params_get_oneparam(MyParams, "navierstokes_params", "uzawa.iterative_solver.bandwidth_method" ))


    STOKES        = sp_get(2*NBSOMM_U + NBSOMM_P, 2*NBSOMM_U + NBSOMM_P, 10)
    STOKES_RHS    =  v_get(2*NBSOMM_U + NBSOMM_P)

    STOKES_bc     = sp_get(2*NBSOMM_U + NBSOMM_P, 2*NBSOMM_U + NBSOMM_P, 10)
    STOKES_RHS_bc =  v_get(2*NBSOMM_U + NBSOMM_P)

    Su     = sp_get(NBSOMM_U, NBSOMM_U,10); # stiff

    UoUx    = sp_get(NBSOMM_U, NBSOMM_U,10)
    VoUy    = sp_get(NBSOMM_U, NBSOMM_U,10)
    UoVx    = sp_get(NBSOMM_U, NBSOMM_U,10)
    VoVy    = sp_get(NBSOMM_U, NBSOMM_U,10)

    UUox    = sp_get(NBSOMM_U, NBSOMM_U,10)
    VUoy    = sp_get(NBSOMM_U, NBSOMM_U,10)
    UVox    = sp_get(NBSOMM_U, NBSOMM_U,10)
    VVoy    = sp_get(NBSOMM_U, NBSOMM_U,10)

    UoUx_p_VoUy_p_UUox = sp_get(NBSOMM_U, NBSOMM_U,10)
    UoVx_p_VoVy_p_VVoy = sp_get(NBSOMM_U, NBSOMM_U,10)


    Bx     = sp_get(NBSOMM_P, NBSOMM_U,10)
    By     = sp_get(NBSOMM_P, NBSOMM_U,10)

    J      = sp_get(NBSOMM_P, NBSOMM_P,10)
    C      = sp_get(NBSOMM_P, NBSOMM_P,10)


    UnUox    = v_get(NBSOMM_U)
    VnUoy    = v_get(NBSOMM_U)
    UnVox    = v_get(NBSOMM_U)
    VnVoy    = v_get(NBSOMM_U)

    UomnUnx  = v_get(NBSOMM_U)
    VomnUny  = v_get(NBSOMM_U)
    UomnVnx  = v_get(NBSOMM_U)
    VomnVny  = v_get(NBSOMM_U)

    Uomn  = v_get(NBSOMM_U)
    Vomn  = v_get(NBSOMM_U)


    RHS_NLIN_U  =  v_get(NBSOMM_U)
    RHS_NLIN_V  =  v_get(NBSOMM_U)

    STOKES_RHS_NLIN  =  v_get(2*NBSOMM_U + NBSOMM_P)


    Un  = v_get(NBSOMM_U); v_zero(Un)
    Vn  = v_get(NBSOMM_U); v_zero(Vn)
    Pn  = v_get(NBSOMM_P); v_zero(Pn)

    Unp1 = v_get(NBSOMM_U)
    Vnp1 = v_get(NBSOMM_U)

    VIT = v_get(2*NBSOMM_U)
    P   = v_get(NBSOMM_P)

    print "Stokes first..."
    # first estimation ...
    (Uo,Vo,Po) = Py_solve2D_stokes( MyElt , MyEltM1 , MyGeom , MyBC , MyRhsFun )
    print "Stokes ... Done"


    # ----- assemblage matrix and rhs ------ 

    J  = assemblage2D_matrix_Mass  ( MyEltM1 , GEOM_2D_geomBase_get(MyGeom) , J  )

    Su   = assemblage2D_matrix_Stiff1( MyElt , MyGeom , Su )
    sp_smlt(Su, kappa, Su)

    Bx = assemblage2D_matrix_Conv_x_elt2elt1( MyElt , MyGeom , MyEltM1 , GEOM_2D_geomBase_get(MyGeom) , Bx )
    By = assemblage2D_matrix_Conv_y_elt2elt1( MyElt , MyGeom , MyEltM1 , GEOM_2D_geomBase_get(MyGeom) , By )

    sp_smlt( Bx, -1.0, Bx)
    sp_smlt( By, -1.0, By)

    UoUx  = assemblage2D_matrix_AUx   ( MyElt , MyGeom , Uo, UoUx )
    VoUy  = assemblage2D_matrix_AUy   ( MyElt , MyGeom , Vo, VoUy )

    UoVx  = assemblage2D_matrix_AUx   ( MyElt , MyGeom , Uo, UoVx )
    VoVy  = assemblage2D_matrix_AUy   ( MyElt , MyGeom , Vo, VoVy )


    UUox  = assemblage2D_matrix_AxU   ( MyElt , MyGeom , Uo, UUox )
    VUoy  = assemblage2D_matrix_AyU   ( MyElt , MyGeom , Uo, VUoy )

    UVox  = assemblage2D_matrix_AxU   ( MyElt , MyGeom , Vo, UVox )
    VVoy  = assemblage2D_matrix_AyU   ( MyElt , MyGeom , Vo, VVoy )


    sp_zero(UoUx_p_VoUy_p_UUox)
    sp_add(UoUx, UoUx_p_VoUy_p_UUox, UoUx_p_VoUy_p_UUox)
    sp_add(VoUy, UoUx_p_VoUy_p_UUox, UoUx_p_VoUy_p_UUox)
    sp_add(UUox, UoUx_p_VoUy_p_UUox, UoUx_p_VoUy_p_UUox)

    sp_zero(UoVx_p_VoVy_p_VVoy);
    sp_add(UoVx, UoVx_p_VoVy_p_VVoy, UoVx_p_VoVy_p_VVoy)
    sp_add(VoVy, UoVx_p_VoVy_p_VVoy, UoVx_p_VoVy_p_VVoy)
    sp_add(VVoy, UoVx_p_VoVy_p_VVoy, UoVx_p_VoVy_p_VVoy)


    sp_zero(STOKES)
    # assemblage of the big matrix "STOKES"
    STOKES = sp_move(Su , 0,0, NBSOMM_U,NBSOMM_U, STOKES,          0,       0)
    STOKES = sp_move(Su , 0,0, NBSOMM_U,NBSOMM_U, STOKES,   NBSOMM_U,NBSOMM_U)

    STOKES = sp_madd(UoUx_p_VoUy_p_UUox, 0,0, NBSOMM_U,NBSOMM_U, STOKES,          0,       0)
    STOKES = sp_madd(UoVx_p_VoVy_p_VVoy, 0,0, NBSOMM_U,NBSOMM_U, STOKES,   NBSOMM_U,NBSOMM_U)

    STOKES = sp_move(VUoy, 0,0, NBSOMM_U,NBSOMM_U, STOKES,        0, NBSOMM_U)
    STOKES = sp_move(UVox, 0,0, NBSOMM_U,NBSOMM_U, STOKES, NBSOMM_U,        0)

    STOKES = sp_move(Bx , 0,0, NBSOMM_P,NBSOMM_U, STOKES, 2*NBSOMM_U,       0)
    STOKES = sp_move(By , 0,0, NBSOMM_P,NBSOMM_U, STOKES, 2*NBSOMM_U,NBSOMM_U)


    STOKES_bc     = sp_copy2(STOKES,     STOKES_bc)

    transform2D_navierstokes_matrix_with_bc( MyElt, MyGeom, GEOM_2D_geomBase_get(MyGeom), MyBC, STOKES_bc)


    # rhs
    Rhs2D_setCurrentSelectedAxe(MyRhsFun, AXEe_X)
    Fu = v_get(NBSOMM_U)
    Fu = assemblage2D_vector_fun( MyElt, MyGeom, MyRhsFun, Fu)

    Rhs2D_setCurrentSelectedAxe(MyRhsFun, AXEe_Y)
    Fv = v_get(NBSOMM_U);
    Fv = assemblage2D_vector_fun( MyElt, MyGeom, MyRhsFun, Fv)


    STOKES_RHS = v_move(Fu, 0, NBSOMM_U, STOKES_RHS,        0) # set Fu into STOKES_RHS
    STOKES_RHS = v_move(Fv, 0, NBSOMM_U, STOKES_RHS, NBSOMM_U) # set Fv into STOKES_RHS



    A  = sp_get(2*NBSOMM_U, 2*NBSOMM_U, 10); sp_zero(A)
    B  = sp_get(  NBSOMM_P, 2*NBSOMM_U, 10); sp_zero(B)

    sp_zero(A)
    sp_zero(B)
    # get A, B
    A = sp_move(STOKES_bc,          0,0, 2*NBSOMM_U,2*NBSOMM_U, A, 0,0)
    B = sp_move(STOKES_bc, 2*NBSOMM_U,0,   NBSOMM_P,2*NBSOMM_U, B, 0,0)


    # factorise now
    Pu    = px_get(2*NBSOMM_U)
    INVPu = px_get(2*NBSOMM_U)

    Pp    = px_get(NBSOMM_P)
    INVPp = px_get(NBSOMM_P)

    LU_A = sp_copy(A)
    LU_P = sp_copy(J)

    # set the matrix with reduced band width 
    LU_A = sp_dopermutationforbandwr(A , Pu, INVPu, BANDWRe_AMD, BANDWRe_PROFILE , BANDWRe_NONSYM, LU_A )

    # set the matrix with reduced band width */
    LU_P = sp_dopermutationforbandwr(J , Pp, INVPp, BANDWRe_AMD, BANDWRe_PROFILE , BANDWRe_NONSYM, LU_P )


    PERM_LU_A = px_get(2*NBSOMM_U)
    PERM_LU_P = px_get(  NBSOMM_P)

    spLUfactor(LU_A, PERM_LU_A, 0.5)
    spLUfactor(LU_P, PERM_LU_P, 0.5)


    F   = v_get(2*NBSOMM_U)
    G   = v_get(  NBSOMM_P)

    # init VIT
    v_move(Uo, 0,NBSOMM_U, VIT,        0)
    v_move(Vo, 0,NBSOMM_U, VIT, NBSOMM_U)


    for n in range(max_steps):

        # get back velocity from the vector VIT
        Un = v_move(VIT,        0,NBSOMM_U, Un,0)
        Vn = v_move(VIT, NBSOMM_U,NBSOMM_U, Vn,0)


        UnUox    = assemblage2D_vector_abx( MyElt , MyGeom , Un , Uo , UnUox   )
        VnUoy    = assemblage2D_vector_aby( MyElt , MyGeom , Vn , Uo , VnUoy   )
        UnVox    = assemblage2D_vector_abx( MyElt , MyGeom , Un , Vo , UnVox   )
        VnVoy    = assemblage2D_vector_aby( MyElt , MyGeom , Vn , Vo , VnVoy   )

        UomnUnx  = assemblage2D_vector_abx( MyElt , MyGeom , v_sub(Uo, Un, Uomn) , Un , UomnUnx )
        VomnUny  = assemblage2D_vector_aby( MyElt , MyGeom , v_sub(Vo, Vn, Vomn) , Un , VomnUny )
        UomnVnx  = assemblage2D_vector_abx( MyElt , MyGeom , v_sub(Uo, Un, Uomn) , Vn , UomnVnx )
        VomnVny  = assemblage2D_vector_aby( MyElt , MyGeom , v_sub(Vo, Vn, Vomn) , Vn , VomnVny )


        v_zero(RHS_NLIN_U)
        RHS_NLIN_U = v_add( RHS_NLIN_U, UomnUnx, RHS_NLIN_U)
        RHS_NLIN_U = v_add( RHS_NLIN_U, VomnUny, RHS_NLIN_U)
        RHS_NLIN_U = v_add( RHS_NLIN_U, UnUox  , RHS_NLIN_U)
        RHS_NLIN_U = v_add( RHS_NLIN_U, VnUoy  , RHS_NLIN_U)

        v_zero(RHS_NLIN_V)
        RHS_NLIN_V = v_add( RHS_NLIN_V, UomnVnx, RHS_NLIN_V)
        RHS_NLIN_V = v_add( RHS_NLIN_V, VomnVny, RHS_NLIN_V)
        RHS_NLIN_V = v_add( RHS_NLIN_V, UnVox  , RHS_NLIN_V)
        RHS_NLIN_V = v_add( RHS_NLIN_V, VnVoy  , RHS_NLIN_V)
	

        STOKES_RHS_NLIN = v_move(RHS_NLIN_U, 0, NBSOMM_U, STOKES_RHS_NLIN,        0) # set RHS_NLIN_U into STOKES_RHS_NLIN
        STOKES_RHS_NLIN = v_move(RHS_NLIN_V, 0, NBSOMM_U, STOKES_RHS_NLIN, NBSOMM_U) # set RHS_NLIN_V into STOKES_RHS_NLIN

        STOKES_RHS_bc =  v_add(STOKES_RHS, STOKES_RHS_NLIN , STOKES_RHS_bc)

        # build the matrix "NAVIER_STOKES"
        transform2D_navierstokes_vector_with_bc( MyElt, MyGeom, GEOM_2D_geomBase_get(MyGeom), MyBC, STOKES, STOKES_RHS_bc)

        # get F and G
        F = v_move(STOKES_RHS_bc,          0,2*NBSOMM_U, F, 0)
        G = v_move(STOKES_RHS_bc, 2*NBSOMM_U,  NBSOMM_P, G, 0)



        OUTER_METHOD = Params_get_oneparam(MyParams, "navierstokes_params", "outer_method")

        if OUTER_METHOD == "UZAWA" :

            navierstokes_resol_uzawa_factOk(LU_A, PERM_LU_A, B, C, LU_P , PERM_LU_P, VIT, P, F, G, Pu, INVPu, Pp, INVPp)

        elif OUTER_METHOD == "PRESSUREMATRIX" :

            navierstokes_resol_pressurematrix_factOk(A, LU_A, PERM_LU_A, B, VIT, P, F, G, Pu, INVPu )
	
        else:

            print "outer method for NavierStokes is \"%s\" \n" % OUTER_METHOD
            error(E_UNKNOWN, "Py_solve2D_NavierStokes_NA")


        # get back velocity from the vector VIT
        Unp1 = v_move(VIT,        0,NBSOMM_U, Unp1,0)
        Vnp1 = v_move(VIT, NBSOMM_U,NBSOMM_U, Vnp1,0)
        # test convergence
        diff = v_norm2( v_sub(Unp1, Un, Unp1) ) + v_norm2( v_sub(Vnp1, Vn, Vnp1) ) 
        print "iter = %d -> diff = %ld " % (n, diff) 

        if diff < eps_steps :
		      break


    # get back velocity from the vector VIT
    Un = v_move(VIT,        0,NBSOMM_U, Un,0)
    Vn = v_move(VIT, NBSOMM_U,NBSOMM_U, Vn,0)

    # clean
    SP_FREE(STOKES);    V_FREE(STOKES_RHS)
    SP_FREE(STOKES_bc); V_FREE(STOKES_RHS_bc)

    SP_FREE(Su)

    SP_FREE(UoUx); SP_FREE(VoUy); SP_FREE(UoVx); SP_FREE(VoVy)
    SP_FREE(UUox); SP_FREE(VUoy); SP_FREE(UVox); SP_FREE(VVoy)

    SP_FREE(UoUx_p_VoUy_p_UUox)
    SP_FREE(UoVx_p_VoVy_p_VVoy)

    SP_FREE(Bx)
    SP_FREE(By)

    SP_FREE(J)
    SP_FREE(C)


    V_FREE(UnUox); V_FREE(VnUoy); V_FREE(UnVox); V_FREE(VnVoy)
    V_FREE(UomnUnx); V_FREE(VomnUny); V_FREE(UomnVnx); V_FREE(VomnVny)

    V_FREE(RHS_NLIN_U)
    V_FREE(RHS_NLIN_V)

    V_FREE(STOKES_RHS_NLIN)

    V_FREE(Unp1)
    V_FREE(Vnp1)

    V_FREE(Fu)
    V_FREE(Fv)

    SP_FREE(A)
    SP_FREE(B)

    V_FREE(VIT)
    V_FREE(F)
    V_FREE(G)

    return (Un,Vn,P)
