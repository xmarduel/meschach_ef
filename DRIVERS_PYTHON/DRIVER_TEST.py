#! /usr/bin/env python

from math import *

import sys

from meschach          import *
from meschach_adds     import *
from meschach_ef       import *
#
#
#
# TEST : U(x,t) = cos(x-t) -> Ux  = -sin(x-t)
#                             Uxx = -cos(x-t)
#                             Ut  =  sin(x-t)
#
#  Ut - k.Uxx + UUx = f(x,t)  ( kappa = 1.0  ) 
#
#-------------------------------------------------------------------

mem_attach_list1();
mem_attach_list2();
mem_attach_list3();
mem_attach_list4();
mem_attach_list5();
mem_attach_list6();
mem_attach_list7();

err_attach_list2();
err_attach_list3();
err_attach_list4();
err_attach_list5();
err_attach_list6();
err_attach_list7();


set_err_flag(EF_JUMP);


eps = 0.0
kap = 1.0
sig = 0.0

def U(x,t):
    return 1
    return  cos(x-t)
def Ux(x,t):
    return 0
    return -sin(x-t)
def Ut(x,t):
    return 0
    return  sin(x-t)
def Uxx(x,t):
    return 0
    return -cos(x-t)
def Uxxxx(x,t):
    return 0
    return  cos(x-t)

def source(x,t):
    return 1
    #return   Ut(x,t) - kap*Uxx(x,t) +                  eps*Uxxxx(x,t)
    return   Ut(x,t) - kap*Uxx(x,t) + U(x,t)*Ux(x,t) + eps*Uxxxx(x,t)
    #return ( sin(x-t) - kap*(-cos(x-t)) - cos(x-t)*sin(x-t) + eps*cos(x-t) )
    #return 0.0


fun1Dex = Fun1D_get();
Fun1D_setFunctionTransientPython(fun1Dex, U); # the solution


def init_condition(x):
    return U(x,0)
def Dinit_condition(x):
    return Ux(x,0)

def bc_left(x,t):
    return  U(x,t);

def bc_right(x,t):
    return  U(x,t);


fp2 = open("OUTPUT_PDE.dat", "w");

MyParams = Params_get();

#-------------------------------------------------------------------   
# LECTURE  IN  INPUT FILE "INPUT_PDE.dat"
# ECRITURE IN OUTOUT FILE "OUTPUT_PDE.dat"
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "input_output_files","fp2", fp2)
#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "finite_elements_params","name_ef", "H3" )    # Type d'EF : "P1","P2", "P3" , "H3", "S3", "S5"

#-physical parameters------------------------------------------------------------------

Params_set_oneparam(MyParams, "physical_params","kappa",    kap ) # kappa
Params_set_oneparam(MyParams, "physical_params","sigma",    sig ) # sigma 
Params_set_oneparam(MyParams, "physical_params","epsilon",  eps ) # epsilon  -k*LAPLACIEN(u) + e.BILAPL(u) + sigma.u = F

#- geometry-----------------------------------------------------------------------------

Params_set_oneparam(MyParams, "geometry_params","meshfile", "NULL" )     #  meshfile ("name.dat") 
Params_set_oneparam(MyParams, "geometry_params","meshtype", "line" )     #
#
Params_set_oneparam(MyParams, "geometry_params","xmin"  , 0.0  ) # xmax
Params_set_oneparam(MyParams, "geometry_params","xmax"  , 2*pi ) # xmax
Params_set_oneparam(MyParams, "geometry_params","nx"    ,  20 ) # nx
Params_set_oneparam(MyParams, "geometry_params","distribution",  "UNIFORM" ) # nx
Params_set_oneparam(MyParams, "geometry_params","periodicity" , PERIODIC_MESHe)

MyIC  = Fun1D_get();
MyDIC = Fun1D_get();
Fun1D_setFunctionPython(MyIC , init_condition ); # ref_e=0
Fun1D_setFunctionPython(MyDIC, Dinit_condition ); # ref_e=0


MyBC = Bc1D_get();
Bc1D_setFunctionTransientPython(MyBC, BC_1De_DIRICHLET, 1, AXEe_X, bc_left ); # ref_s=1 (left) 
Bc1D_setFunctionTransientPython(MyBC, BC_1De_DIRICHLET, 2, AXEe_X, bc_right); # ref_s=2 (right)

Bc1D_setBcType(MyBC, BC_1De_DIRICHLET, 1, AXEe_X);
Bc1D_setBcType(MyBC, BC_1De_DIRICHLET, 2, AXEe_X); # BC_1De_CAUCHY, BC_1De_DIRICHLET, BC_1De_NEUMANN


MyRhsFun = Rhs1D_get();
Rhs1D_setFunctionTransientPython(MyRhsFun, 0, AXEe_X, source ); # ref_e=0

#############################################################################################
Params_set_staticparam(MyParams, 0);
#############################################################################################

MyElt  = elt1D_get(Params_get_oneparam(MyParams, "finite_elements_params", "name_ef"));

#--------------------------------------------------------------------

MyGeom = Geom1D_get_fromscratch(MyElt,
    Params_get_oneparam(MyParams, "geometry_params", "xmin"),
    Params_get_oneparam(MyParams, "geometry_params", "xmax"),
    Params_get_oneparam(MyParams, "geometry_params", "nx"),
    Params_get_oneparam(MyParams, "geometry_params", "periodicity"),
    Params_get_oneparam(MyParams, "geometry_params", "distribution"))

#-------------------------------------------------------------------
#
NBSOMM = GEOM_1D_NBSOMM_get(MyGeom)

MASS   = sp_get(NBSOMM, NBSOMM, 5)
MASS   = assemblage1D_matrix_Mass (MyElt, MyGeom, MASS);

# initial solution
U_o = v_get(NBSOMM)
U_o = build_vec_from_function1D( MyElt, MyGeom, MyIC, MyDIC, U_o )

RHS_FUN     = v_get(NBSOMM)
RHS_UUx     = v_get(NBSOMM)
RHS_UU      = v_get(NBSOMM)

tmp1 = v_get(NBSOMM)
tmp2 = v_get(NBSOMM)

#---------------------------------------------------------------------

# term from rhs function
RHS_FUN = assemblage1D_vector_fun( MyElt, MyGeom, MyRhsFun , RHS_FUN )   #  RHS_FUN   = DT * F_n

# term from convection
RHS_UUx = assemblage1D_vector_abx( MyElt, MyGeom, U_o, U_o , RHS_UUx )   #  RHS_UUx   = [ U_o * GRAD( U_o ) ]

# term from convection
RHS_UU  = assemblage1D_vector_ab ( MyElt, MyGeom, U_o, U_o , RHS_UU  )   #  RHS_UU    = [ U_o * U_o ]

#
# TESTS
# -----

LLT_MASS = sp_copy(MASS)
spCHfactor(LLT_MASS)

# TESTS
# -----

print "Uo"
#v_foutput(sys.stdout, U_o)
v_foutput(sys.stdout, sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), U_o, tmp1) )

print "RHS_FUN"
v_foutput(sys.stdout, RHS_FUN)
tmp2 = spCHsolve(LLT_MASS, RHS_FUN, tmp2)
v_foutput(sys.stdout, sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), tmp2, tmp1) )

print "RHS_UUx"
#v_foutput(sys.stdout, RHS_UUx)
tmp2 = spCHsolve(LLT_MASS, RHS_UUx, tmp2)
v_foutput(sys.stdout, sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), tmp2, tmp1) )

print "RHS_UU"
#v_foutput(sys.stdout, RHS_UU)
tmp2 = spCHsolve(LLT_MASS, RHS_UU, tmp2)
v_foutput(sys.stdout, sp_mv_mlt(GEOM_1D_EF_to_WORLD_get(MyGeom), tmp2, tmp1) )

