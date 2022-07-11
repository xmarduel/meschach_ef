#! /usr/bin/env python

import sys
from   math import cos, sin, sqrt, pi, exp
import time
import threading
import json

from meschach            import *
from meschach_spooles    import *
from meschach_lapack     import *
from meschach_adds       import *
from meschach_cpgplot    import *
from meschach_vogle      import *
from meschach_libsciplot import *
from meschach_ef         import *

#-------------------------------------------------------------------

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
    #mem_attach_list8()

    err_attach_list2()
    err_attach_list3()
    err_attach_list4()
    err_attach_list5()
    err_attach_list6()
    err_attach_list7()
    err_attach_list8()

    warn_attach_list2()
    warn_attach_list3()
    warn_attach_list4()
    warn_attach_list5()
    warn_attach_list6()
    warn_attach_list7()
    warn_attach_list8()


    set_err_flag(EF_JUMP)
    set_warn_flag(EF_SILENT)

    #-------------------------------------------------------------------

    def sol(x):
        #return (x-1)*(x-1)
        #return 1-x
        #return x*(x-1) +2
        #return x*x
        #return 1
        return cos(2*pi*x)
        return (exp(x)-1)/(exp(1)-1)

    def Dsol(x):
        #return 2*(x-1)
        #return 1
        #return 2*x - 1
        #return 2*x
        return -2*pi*sin(2*pi*x)
        return exp(x)/(exp(1)-1)

    def DDsol(x):
        #return -2
        #return 0
        #return 2
        #return 2
        return -4*pi*pi*cos(2*pi*x)
        return (exp(x))/(exp(1)-1)


    def adv(x):
        return 1

    def src(x):
        #return 0
        return  - DDsol(x)                   # -DD
        #return - DDsol(x) + sol(x)          # -DD + Id
        #return - DDsol(x) + Dsol(x)         # -DD + D
        #return - DDsol(x) + sol(x)*Dsol(x)  # -DD + I.D
        #return - DDsol(x) + sol(x)*sol(x)   # -DD + I.I


    def Nsol_right(x):  # Neumann BC :  at X_RIGHT (REF_S = 2)
        return Dsol(x)

    def Nsol_left(x): # Neumann BC: t X_LEFT (REF_S = 1)
        return -Dsol(x)  #because df is seen from the right to the left


    def R1sol_right(x):  # Robin BC : df + R1.f = R2  at X_RIGHT (REF_S = 2)
        return 1.000     # we choose R1 = 1 -> see R2

    def R2sol_right(x): # Robin BC: df + R1.f = R2 at X_RIGHT (REF_S = 2)
        return Dsol(x) + sol(x)  # R1 = 1 -> R2 = df + f

    def R1sol_left(x):  # Robin BC : df + R1.f = R2   at X_LEFT (REF_S = 1)
        return 1.000    # we choose R1 = 1 -> see R2

    def R2sol_left(x): # Robin BC: df + R1.f = R2 at X_LEFT (REF_S = 1)
        return -Dsol(x) + sol(x)  # R1 = 1 -> R2 = - df + f because df is seen from the right to the left


    def Csol_right(x): # cauchy BC : df + A.f = 0  (REF_S = 2)
        return -Dsol(x)/sol(x)  # A =  df/f

    def Csol_left(x): # cauchy BC : df + A.f = 0  (REF_S = 1)
        return  Dsol(x)/sol(x)  # A = -df/f because df is seen from the right to the left


    # ----------------------------------------------------------------
    def CONVECTION_SOLEXACTE(x):
        kappa = 0.01
        sol = ( exp(x/kappa) -1.0 ) / ( exp(1.0/kappa) -1.0 )
        return sol

    def CONVECTION_SOLEXACTE_DISCRETE(x, j, M):
        kapa = 0.01
        M    = 20
        Pe = ( 1.0/M )/( 2*kappa )

        if Pe >  1.0 :  sol = ( pow((1+Pe)/(1-Pe),j) -1.0 )/( pow((1+Pe)/(1-Pe),M) - 1.0 )
        if Pe == 1.0 :  sol = 0.0
        if Pe <  1.0 :  sol = ( pow((1+Pe)/(1-Pe),j) -1.0 )/( pow((1+Pe)/(1-Pe),M) - 1.0 )

        return sol
    #-------------------------------------------------------------------

    MyBC = Bc1D_get()

    Bc1D_setBcType(MyBC, BC_1De_DIRICHLET, 1, AXEe_X)
    Bc1D_setBcType(MyBC, BC_1De_DIRICHLET, 2, AXEe_X) # BC_1De_CAUCHY, BC_1De_DIRICHLET, BC_1De_NEUMANN, BC_1De_ROBIN

    Bc1D_setFunctionPython(MyBC, BC_1De_DIRICHLET, 1, AXEe_X, sol)
    Bc1D_setFunctionPython(MyBC, BC_1De_DIRICHLET, 2, AXEe_X, lambda x : sol(x) + 0 )

    Bc1D_setFunctionPython(MyBC, BC_1De_NEUMANN, 1, AXEe_X, Nsol_left)
    Bc1D_setFunctionPython(MyBC, BC_1De_NEUMANN, 2, AXEe_X, Nsol_right)

    Bc1D_setFunctionPython(MyBC, BC_1De_CAUCHY, 1, AXEe_X, Csol_left)
    Bc1D_setFunctionPython(MyBC, BC_1De_CAUCHY, 2, AXEe_X, Csol_right)

    Bc1D_setFunctionPython (MyBC, BC_1De_ROBIN, 1, AXEe_X, R1sol_left)
    Bc1D_setFunctionPython2(MyBC, BC_1De_ROBIN, 1, AXEe_X, R2sol_left)

    Bc1D_setFunctionPython (MyBC, BC_1De_ROBIN, 2, AXEe_X, R1sol_right)
    Bc1D_setFunctionPython2(MyBC, BC_1De_ROBIN, 2, AXEe_X, R2sol_right)

    Bc1D_checkBC(MyBC)



    MyRhsFun = Rhs1D_get()
    Rhs1D_setFunctionPython(MyRhsFun, 0, AXEe_X, src ) # ref_e=0, axe=1


    MyAdvFun = Adv1D_get()
    Adv1D_setFunctionPython(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_X, adv ) # ref_e=0, axe=1


    #-------------------------------------------------------------------

    MyParams = Params_get()

    config = str(json.dumps( {
        "PDE" : "LAPLACIAN",   # "LAPLACIAN", "CONVECTION-DIFFUSION", "BURGERS", "HELMHOLZ",

        "PDE_PARAMATERS" : {
            "KAPPA"    : 1.000,
            "SIGMA"    : 0.0,
            "EPSILON"  : 0.0
        },

        "PDE_RESOLUTION" : {
            "EF" : "S4" ,         # "P1", "P2", "H3", "S2", "S3", "S4", "S5"

            "LAPLACIAN_ALGO" : {
                "METHOD" : "ITERATIVE-METHOD",

                "ITERATIVE-METHOD" : {
                    "METHOD"          : "CG" ,       # "CG", "CGS", "BiCGStab"
                    "PRECONDITIONER"  : "NULL" ,     # "NULL", "ICH", "ILU"
                    "PARAMETERS" : {
                        "NMAX_ITER" : 500,
                        "EPSILON"   : 1.0e-14
                    },
                    "OUTPUT" : {
                        "STDOUT" : True,
                        "FILE"   : "residuals.dat"
                    }
                },

                "DIRECT-METHOD" : {
                    "BANDWIDTHREDUCTION" : {
                        "OPTION" :         "PROFILE",
                        "IMPLEMENTATION" : "MESCHACH"
                    }
                }
            }
        },

        "OUTPUT_LOG_FILE"    : "output.txt"

    } ))

    MyParams = Params_append_from_string(MyParams, config)

    # geom  data
    config = str(json.dumps( {
        "COORDS_DIMS"    : 1,
        "MESH_1D"        : {
            "MESH_SPEC": "MESHDATADEFINITION",

            "MESHFILEDEFINITION" : {
              "MESHFILE": "NULL",
              "MESHFILE"     : "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/1D/mesh1D.line",
              "MESHNAME"     : "MESH_50",
              "MESHTYPE"     : "line"
            },

            "MESHDATADEFINITION" : {
                "XMIN"   : 0.0,
                "XMAX"   : 1.0,
                "NX"     : 50,
                "DISTRIBUTION" : "UNIFORM",
                "PERIODIC" : False
            },
        }
    } ))

    MyParams = Params_append_from_string(MyParams, config)

    # plotting data
    config = str(json.dumps( {
        "COORDS_DIMS"    : 1,
        "GRAPHICS_1D_OUTPUT" : {
            "GNUPLOT"    : False,
            "MATLAB"     : False,
            "VTK"        : False,
            "SILO"       : False
        }
    } ))

    MyParams = Params_append_from_string(MyParams, config)

    # graphics datas
    config = str(json.dumps( {
        "GRAPHICS_1D" : {
            #"ENGINE" : "VOGLE",
            "ENGINE" : "GRAPH",
            "DRIVER" : "/XWINDOW" ,
            "WINDOW_SIZE" : [700, 500] ,
            "LABELS_SIZE" : 1.1 ,

            "SUB_WINDOWS" : [
                {
                "IDXS"         : [1,1] , # ARRAY POSITION
                "LEGEND"       : "Solution" ,
                "LEGEND_AXE_X" : "X" ,
                "LEGEND_AXE_Y" : "Y" ,
                "MARKER"       : 0 ,
                "MARKER_END"   : 0 ,
                "VIEWPORT"     : { "X_MIN":0.0, "X_MAX":1.0, "Y_MIN":-1.5 , "Y_MAX":2.5 }
                }
            ]
        }
    } ))

    MyParams = Params_append_from_string(MyParams, config)


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


    Geom1D_foutput(sys.stdout,MyGeom)
    n = Geom1D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X)

    #--------------------------------------------------------------------

    PROBLEM = Params_get_oneparam(MyParams, "main_problem", "NULL")
    print(PROBLEM)


    if   PROBLEM == "LAPLACIAN" :

        SOL = solve1D_laplacian( MyElt , MyGeom , MyBC, MyRhsFun )

    elif PROBLEM == "BIPAPLACIAN"  :

        SOL = solve1D_bilaplacian( MyElt , MyGeom , MyBC, MyRhsFun )

    elif PROBLEM == "CONVECTION-DIFUSION":

        SOL = solve1D_convection_diffusion( MyElt , MyGeom , MyBC, MyRhsFun , MyAdvFun )

    elif PROBLEM == "HELMHOLZ"  :

        SOL = solve1D_helmholz( MyElt , MyGeom , MyBC, MyRhsFun )

    elif PROBLEM == "BURGERS"  :

        SOL = solve1D_burgers( MyElt , MyGeom , MyBC, MyRhsFun )

    else:

        print("Problem \"%s\" not yet implemented" % PROBLEM)
        sys.exit()




    #----------------------- graphics output -----------------------------

    #graphics1D_output_vec  ( MyElt, MyGeom, SOL       , "Solution_Approc.dat")
    #graphics1D_output_fun  ( MyElt, MyGeom, SOLEXACTE , "Solution_Exacte.dat")
    #graphics1D_output_funcd( MyElt, MyGeom, SOLAPPEXA , "Solution_ApproE.dat")

    #----------------------- graphics interative -------------------------------

    if Params_get_oneparam(MyParams,"graphics_output_params","GNUPLOT") == 1 :

        graphics1D_gnuplot_write_scriptscript_1D("SolApproch1D.dat", None, None, 100,
            Params_get_oneparam(MyParams, "miscellaneous_params","iter_file"))


    if Params_get_oneparam(MyParams,"graphics_output_params","GNUPLOT") == 1 :
       graphics1D("gnuplot", MyElt, MyGeom, SOL, "SolApproch1D")
    if Params_get_oneparam(MyParams,"graphics_output_params","SILO") == 1 :
       graphics1D("silo"   , MyElt, MyGeom, SOL, "SolApproch1D")
    if Params_get_oneparam(MyParams,"graphics_output_params","VTK") == 1 :
       graphics1D("vtk"    , MyElt, MyGeom, SOL, "SolApproch1D")



    #print("SOL")
    #v_foutput(sys.stdout, SOL)

    GeomP1  = Geom1D_getP1geom_from(MyElt, MyGeom)
    MESH_P1 = GEOM_1D_XSOMM_get(GeomP1)
    SOL_P1  = build_vec_world_from_vec_ef_1D( MyElt, MyGeom, SOL)

    #print("SOL_P1")
    #v_foutput(sys.stdout, SOL_P1)

    if Params_get_oneparam(MyParams,"graphics_interactiv1D_params","ENGINE") == "GRAPH" :
        graphics1D("graph"    , MyElt, GeomP1, SOL_P1, "SolApproch1D")


    if Params_get_oneparam(MyParams,"graphics_interactiv1D_params","ENGINE") == "PGPLOT" :

        graphics1D_cpgplot_initialize(Params_get_oneparam(MyParams,"graphics_interactiv1D_params","DRIVER"),
                                      Params_get_oneparam(MyParams,"graphics_interactiv1D_params","SIZE_WINDOW_X"),
                                      Params_get_oneparam(MyParams,"graphics_interactiv1D_params","SIZE_WINDOW_Y"), 1,1)
        graphics1D_cpgplot_title("SOLUTION")
        graphics1D_cpgplot_legend(1,1, "SOLUTION")
        graphics1D_cpgplot_labelsize(1,1, Params_get_oneparam(MyParams,"graphics_interactiv1D_params","SIZE_LABELS"))
        graphics1D_cpgplot_window(1,1, Params_get_oneparam2(MyParams,"graphics_interactiv1D_params","WINDOW_X_MIN",1,1),
                                       Params_get_oneparam2(MyParams,"graphics_interactiv1D_params","WINDOW_X_MAX",1,1),
                                       Params_get_oneparam2(MyParams,"graphics_interactiv1D_params","WINDOW_Y_MIN",1,1),
                                       Params_get_oneparam2(MyParams,"graphics_interactiv1D_params","WINDOW_Y_MAX",1,1))
        graphics1D_cpgplot_setbox(1,1, 1)
        graphics1D_cpgplot_curvedata(1, 1, PGPLOTe_COLOR_BLUE1, MESH_P1, SOL_P1)

        t1 = threading.Thread( target=Py_pgplot_curve_with_xt_toolkit )
        t1.start()


    if Params_get_oneparam(MyParams,"graphics_interactiv1D_params","ENGINE") == "VOGLE" :

        graphics1D_vopl_initialize(Params_get_oneparam(MyParams,"graphics_interactiv1D_params","DRIVER"),
                                    Params_get_oneparam(MyParams,"graphics_interactiv1D_params","SIZE_WINDOW_X"),
                                    Params_get_oneparam(MyParams,"graphics_interactiv1D_params","SIZE_WINDOW_Y"), 1,1, VOPL_CURVE)
        graphics1D_vopl_title("SOLUTION")
        graphics1D_vopl_legend(1,1, "SOLUTION")
        graphics1D_vopl_labelsize(1,1, Params_get_oneparam(MyParams,"graphics_interactiv1D_params","SIZE_LABELS"))
        graphics1D_vopl_window(1,1,  Params_get_oneparam2(MyParams,"graphics_interactiv1D_params","WINDOW_X_MIN",1,1),
                                     Params_get_oneparam2(MyParams,"graphics_interactiv1D_params","WINDOW_X_MAX",1,1),
                                     Params_get_oneparam2(MyParams,"graphics_interactiv1D_params","WINDOW_Y_MIN",1,1),
                                     Params_get_oneparam2(MyParams,"graphics_interactiv1D_params","WINDOW_Y_MAX",1,1))
        graphics1D_vopl_setbox(1,1, 1)
        graphics1D_vopl_curvedata(1, 1, VOGLEe_COLOR_BLUE, MESH_P1, SOL_P1)

        t1 = threading.Thread( target=Py_vopl_curve_with_xt_toolkit )
        t1.start()


    if Params_get_oneparam(MyParams,"graphics_interactiv1D_params","ENGINE") == "LIBSCIPLOT" :

        graphics1D_libsciplot_initialize(Params_get_oneparam(MyParams,"graphics_interactiv1D_params","DRIVER"),
                                         Params_get_oneparam(MyParams,"graphics_interactiv1D_params","SIZE_WINDOW_X"),
                                         Params_get_oneparam(MyParams,"graphics_interactiv1D_params","SIZE_WINDOW_Y"), 1,1)
        graphics1D_libsciplot_curvedata1(1,1, LIBSCIPLOTe_COLOR_BLUE, MESH_P1, SOL_P1, -1,-1 )

        graphics1D_libsciplot_framecolor(1,1, LIBSCIPLOTe_COLOR_RED)
        graphics1D_libsciplot_framecolor(1,1, LIBSCIPLOTe_COLOR_GREEN)

        graphics1D_libsciplot_title("SOLUTION")
        graphics1D_libsciplot_legend(1,1, "SOLUTION")
        graphics1D_libsciplot_labelsize(1,1, Params_get_oneparam(MyParams,"graphics_interactiv1D_params","SIZE_LABELS") )
        graphics1D_libsciplot_window(1,1,Params_get_oneparam2(MyParams,"graphics_interactiv1D_params","WINDOW_X_MIN",1,1),
                                         Params_get_oneparam2(MyParams,"graphics_interactiv1D_params","WINDOW_X_MAX",1,1),
                                         Params_get_oneparam2(MyParams,"graphics_interactiv1D_params","WINDOW_Y_MIN",1,1),
                                         Params_get_oneparam2(MyParams,"graphics_interactiv1D_params","WINDOW_Y_MAX",1,1))
        graphics1D_libsciplot_setbox(1,1, 1)

        t1 = threading.Thread( target=Py_libsciplot_curve_with_xt_toolkit )
        t1.start()


    #---------------------------------------------------------------------------
    has_exact_sol = True

    if has_exact_sol:

        fun1D = Fun1D_get()
        Fun1D_setFunctionPython(fun1D, sol)

        WORLD = build_vec_world_from_vec_ef_1D(MyElt, MyGeom, SOL);
        WORLD_exact = build_vec_world_from_function1D(MyElt, MyGeom, fun1D, None, None)

        print("WORLD (EF)")
        v_foutput(sys.stdout, WORLD)
        print("WORLD EXACT")
        v_foutput(sys.stdout, WORLD_exact)

        print("diff solexacte-solapprochee in EF = %le" % v_norm2( v_sub(WORLD, WORLD_exact, None) ))

        FUN_1D_FREE(fun1D)
        V_FREE(WORLD_exact)
        V_FREE(WORLD)



    # free memory
    # -----------

    ELT_1D_FREE(MyElt)

    GEOM_1D_FREE(MyGeom)
    BC_1D_FREE(MyBC)
    RHS_1D_FREE(MyRhsFun)
    ADV_1D_FREE(MyAdvFun)

    V_FREE(SOL)
    V_FREE(SOL_P1)
    GEOM_1D_FREE(GeomP1)

    PARAMS_FREE(MyParams)

    #---------------------------------------------------------------------

    mem_info_file(sys.stdout,0)

    #mem_info_file(sys.stdout, MY_LIST1)
    #mem_info_file(sys.stdout, MY_LIST2)
    #mem_info_file(sys.stdout, MY_LIST3)
    #mem_info_file(sys.stdout, MY_LIST4)
    #mem_info_file(sys.stdout, MY_LIST5)
    #mem_info_file(sys.stdout, MY_LIST6)
    #mem_info_file(sys.stdout, MY_LIST7)

#----------------------------------------------------------------------

if __name__ == '__main__':
    if len(sys.argv) > 1:
        import profile
        profile.run('run_test()')
    else:
        run_test()

#----------------------------------------------------------------------

