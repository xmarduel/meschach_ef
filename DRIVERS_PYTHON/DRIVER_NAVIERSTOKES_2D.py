
from sys  import *
from math import *

from meschach         import *
from meschach_adds    import *
from meschach_ef      import *
from meschach_cpgplot import *
from meschach_spooles import *

from ALGO_NAVIERSTOKES_2D import *

#-------------------------------------------------------------------

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


TEST = 'KOVASZNAY'
#TEST = 'CANAL_WITH_BOULE'
#TEST = 'CN41'
#TEST = 'CAVITY'

	
Re = 400.0

def source_u(x,y):
    return 0.0

def source_v(x,y):
    return 0.0

def bc0(x,y):
    return 0.0


MyBC = Bc2D_get()


if TEST == 'CN41':

	#
	#           1
	#   *----------------*
 	#   |                |
  	#   |                |
 	#   |                |
 	#   |                *-----------------------*
 	#   | 2                                      | 3
  	#   |                *-----------------------*
 	#   |                |
  	#   |                |
 	#   |                |
	#   *----------------*  
	#          1
	#
	 
    CN = 4.0

    def bcENTRANCE(x,y):
        return (CN*CN - y*y)/(CN*CN*CN)
    def bcEXIT(x,y):
        return (1.0 - y*y)

		  
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 3, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 4, AXEe_X )

    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_NEUMANN, 3, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 4, AXEe_Y )


    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_X, bc0 )    # u 0 on boundaries
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_X, bcENTRANCE ) # u elliptic on entrance
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_X, bcEXIT ) # u elliptic on exit

    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_Y, bc0) # v always 0 on boundaries
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_Y, bc0) # v always 0 on boundaries
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 3, AXEe_Y, bc0) # v always 0 on boundaries
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 4, AXEe_Y, bc0) # v always 0 on boundaries


    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 1, AXEe_X, bc0 )
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 2, AXEe_X, bc0 )
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 3, AXEe_X, bc0 )
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 4, AXEe_X, bc0 )

    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 1, AXEe_Y, bc0)
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 2, AXEe_Y, bc0)
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 3, AXEe_Y, bc0)
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 4, AXEe_Y, bc0)

elif TEST == 'KOVASZNAY':

    pi = acos(-1.0)
    Re = 40.0
    L  = Re/2.0 - sqrt(Re*Re/4 + 4*pi*pi)
    print("L = ", L)

    def u(x,y):
        return 1.0 - exp(L*x)*cos(2*pi*y)
    def dudx(x,y):
        return  - L * exp(L*x)*cos(2*pi*y)

    def v(x,y):
        return (L / (2*pi)) * exp(L*x)*sin(2*pi*y)
    def dvdx(x,y):
        return (L*L / (2*pi)) * exp(L*x)*sin(2*pi*y)
	 
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_X )
    #Bc2D_setBcType(MyBC, BC_2De_NEUMANN, 2, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 3, AXEe_X )

    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_Y )
    #Bc2D_setBcType(MyBC, BC_2De_NEUMANN, 2, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 3, AXEe_Y )

    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_X, u ) # input flow
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_X, u ) # dudx output flow
    #Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 2, AXEe_X, dudx ) # dudx output flow
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_X, u ) # u on the sides

    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_Y, v) # input flow
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_Y, v) # output flow
    #Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 2, AXEe_Y, dvdx) # output flow
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_Y, bc0) # v = 0 on sides

elif TEST == 'CANAL_WITH_BOULE':

    def bcEXIT(x,y):
        return (1.0 - y*y)
	 
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_NEUMANN  , 3, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 4, AXEe_X )

    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_NEUMANN  , 3, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 4, AXEe_Y )


    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_X, bc0 )    # u 0 on boundaries
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_X, bcEXIT ) # u elliptic on entrance
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_X, bcEXIT ) # u elliptic on exit

    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_Y, bc0) # v always 0 on boundaries
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_Y, bc0) # v always 0 on boundaries
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_Y, bc0) # v always 0 on boundaries
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 4, AXEe_Y, bc0) # v always 0 on boundaries


    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 1, AXEe_X, bc0 )
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 2, AXEe_X, bc0 )
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 3, AXEe_X, bc0 )
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 4, AXEe_X, bc0 )

    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 1, AXEe_Y, bc0)
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 2, AXEe_Y, bc0)
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 3, AXEe_Y, bc0)
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 4, AXEe_Y, bc0)

elif TEST == 'CAVITY':

    Re = 400.0
    
    def bc_u1(x,y):
        return 1.0
        return 4.0*x*(1.0-x)
		  
    def bc_u2(x,y):
        return 0.0
        return 16.0*x*(1.0-x)
	 
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 3, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 4, AXEe_X )

    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 3, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 4, AXEe_Y )


    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_X, bc_u1 )    # u 0 on boundaries
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_X, bc0 ) # u elliptic on entrance
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_X, bc_u2 ) # u elliptic on exit
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 4, AXEe_X, bc0) # v always 0 on boundaries

    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_Y, bc0) # v always 0 on boundaries
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_Y, bc0) # v always 0 on boundaries
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_Y, bc0) # v always 0 on boundaries
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 4, AXEe_Y, bc0) # v always 0 on boundaries
	 
else:

    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_NEUMANN  , 3, AXEe_X )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 4, AXEe_X )
    #
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 2, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_NEUMANN  , 3, AXEe_Y )
    Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 4, AXEe_Y )
    #
    #
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_X, bc0 )
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_X, bcEXIT )
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_X, bcEXIT )
    #Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_X, bc_u )
    #Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_X, bc0 )
    #Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_X, bc_u )
    #Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 4, AXEe_X, bc0 )
    #
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 1, AXEe_Y, bc0)
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 2, AXEe_Y, bc0)
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 3, AXEe_Y, bc0)
    Bc2D_setFunctionPython(MyBC, BC_2De_DIRICHLET, 4, AXEe_Y, bc0)
    #
    #
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 1, AXEe_X, bc0 )
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 2, AXEe_X, bc0 )
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 3, AXEe_X, bc0 )
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 4, AXEe_X, bc0 )
    #
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 1, AXEe_Y, bc0)
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 2, AXEe_Y, bc0)
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 3, AXEe_Y, bc0)
    Bc2D_setFunctionPython(MyBC, BC_2De_NEUMANN, 4, AXEe_Y, bc0)


MyRhsFun = Rhs2D_get()
Rhs2D_setFunctionPython(MyRhsFun, 0, AXEe_X, source_u)
Rhs2D_setFunctionPython(MyRhsFun, 0, AXEe_Y, source_v)

#-------------------------------------------------------------------   

MyParams = Params_get()

#--------------------------------------------------------------------------------------
Params_set_oneparam(MyParams, "finite_elements_params","name_ef", "P2" )    # Type d'EF : "P1","P2","P3"

Params_set_oneparam(MyParams, "main_problem","NULL", "NAVIER-STOKES" )  # Laplacian Stokes NavierStokes

Params_set_oneparam(MyParams, "navierstokes_params", "method", "NEWTON-EXACT" ) # "NEWTON-EXACT", "NEWTON-APPROCH"
Params_set_oneparam(MyParams, "navierstokes_params", "max_steps", 10 ) 
Params_set_oneparam(MyParams, "navierstokes_params", "eps_steps", 1.e-12)
Params_set_oneparam(MyParams, "navierstokes_params", "outer_method", "UZAWA" ) # "UZAWA", "PRESSUREMATRIX"

Params_set_oneparam(MyParams, "navierstokes_params", "uzawa.rho", 1.0/Re)
Params_set_oneparam(MyParams, "navierstokes_params", "uzawa.innerloop_method", "DIRECT-METHOD" )  # "DIRECT-METHOD" "ITERATIV-METHOD"
Params_set_oneparam(MyParams, "navierstokes_params", "uzawa.innerloop_solver.max_steps", 1000 )
Params_set_oneparam(MyParams, "navierstokes_params", "uzawa.innerloop_solver.eps_steps", 1.e-12)
Params_set_oneparam(MyParams, "navierstokes_params", "uzawa.innerloop_solver.bandwidth_option", BANDWRe_PROFILE )  # 
Params_set_oneparam(MyParams, "navierstokes_params", "uzawa.innerloop_solver.bandwidth_method", BANDWRe_SPOOLES )  # 

Params_set_oneparam(MyParams, "navierstokes_params", "pressurematrix.innerloop_method", "DIRECT-METHOD" )  # "DIRECT-METHOD" "ITERATIV-METHOD"
Params_set_oneparam(MyParams, "navierstokes_params", "pressurematrix.innerloop_solver.max_steps", 1000 )
Params_set_oneparam(MyParams, "navierstokes_params", "pressurematrix.innerloop_solver.eps_steps", 1.e-12)
Params_set_oneparam(MyParams, "navierstokes_params", "pressurematrix.innerloop_solver.bandwidth_option", BANDWRe_PROFILE )  # 
Params_set_oneparam(MyParams, "navierstokes_params", "pressurematrix.innerloop_solver.bandwidth_method", BANDWRe_SPOOLES )  # 

#--------------------------------------------------------------------------------------

Params_set_oneparam(MyParams, "geometry_params","meshfile", "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/2D/kovasznay1.msh" )  #  Mesh File ("name.dat") "MESH_P1.quad2"
#Params_set_oneparam(MyParams, "geometry_params","meshfile", "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/2D/kovasznay2.msh" )  #  Mesh File ("name.dat") "MESH_P1.quad2"
#Params_set_oneparam(MyParams, "geometry_params","meshfile", "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/2D/CANAL_6.emc2" )   #  Mesh File ("name.dat") "MESH_P1.quad2"
#Params_set_oneparam(MyParams, "geometry_params","meshfile", "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/2D/CANAL_7.emc2" )   #  Mesh File ("name.dat") "MESH_P1.quad2"
#Params_set_oneparam(MyParams, "geometry_params","meshfile", "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/2D/CN41_16.emc2" )    #  Mesh File ("name.dat") "MESH_P1.quad2"
#Params_set_oneparam(MyParams, "geometry_params","meshfile", "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/EF_MESHES/2D/CUBE_11.emc2" )  #  Mesh File ("name.dat") "MESH_P1.quad2"CUBE_11.emc2
#Params_set_oneparam(MyParams, "geometry_params","meshtype", "quad2" )     #
#Params_set_oneparam(MyParams, "geometry_params","meshtype", "emc2" )     #
Params_set_oneparam(MyParams, "geometry_params","meshtype", "gmsh" )     #
Params_set_oneparam(MyParams, "geometry_params","meshname", "MESH_40" )     # only for "quad" files

Params_set_oneparam(MyParams, "graphics_output_params","GNUPLOT",     0) # GNUPLOT
Params_set_oneparam(MyParams, "graphics_output_params","GRAPH",       0) # GRAPH
Params_set_oneparam(MyParams, "graphics_output_params","MATLAB",      0) # MATLAB
Params_set_oneparam(MyParams, "graphics_output_params","VTK",         1) # VTK
Params_set_oneparam(MyParams, "graphics_output_params","SILO",        1) # SILO
Params_set_oneparam(MyParams, "graphics_output_params","PGPLOT",      0) # PGPLOT
Params_set_oneparam(MyParams, "graphics_output_params","VOGLE",       0) # VOGLE
Params_set_oneparam(MyParams, "graphics_output_params","LIBSCIPLOT",  0) # LIBSCIPLOT   (0=NO;1=YES)

#-physical parameters------------------------------------------------------------------

Params_set_oneparam(MyParams, "physical_params","epsilon",    0.0000) # epsilon
Params_set_oneparam(MyParams, "physical_params","sigma",      0.0   ) # sigma
Params_set_oneparam(MyParams, "physical_params","kappa",     1.0/Re ) # kappa  -k*LAPLACIEN(u)+ e.BILAPL(u) +sigma.u =F


Params_set_staticparam(MyParams, 0)


MyElt  = elt2D_get(Params_get_oneparam(MyParams, "finite_elements_params", "name_ef"))

elt_P3 = elt2D_get("P3")
elt_P2 = elt2D_get("P2")
elt_P1 = elt2D_get("P1")

if   "P3" == ELT_2D_name_ef_get(MyElt) :
    MyEltM1 = elt_P2
elif "P2" == ELT_2D_name_ef_get(MyElt) :
    MyEltM1 = elt_P1
elif "P1b"== ELT_2D_name_ef_get(MyElt) :
    MyEltM1 = elt_P1
elif "P1" == ELT_2D_name_ef_get(MyElt) :
    MyEltM1 = elt_P1
else :
    pass


#------------------------------------------------------------------------------------------

MyGeom = Geom2D_get(MyElt, 
        Params_get_oneparam(MyParams, "geometry_params", "meshfile"),
        Params_get_oneparam(MyParams, "geometry_params", "meshname"),
        Params_get_oneparam(MyParams, "geometry_params", "meshtype"))

Geom2D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X)  
Geom2D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_Y) 

graphics_geom2D_view("X11", MyGeom, "dummy")
graphics_geom2D_view("gnuplot", MyGeom, "geometry_view")

#------------------------------------------------------------------------------------------

PROBLEM = Params_get_oneparam(MyParams, "main_problem", "NULL")
print("PROBLEM is ", PROBLEM)
METHOD = Params_get_oneparam(MyParams, "navierstokes_params", "method")
print("METHOD is ", METHOD)


if   PROBLEM == "LAPLACIAN" :

    SOL = solve2D_laplacien( MyElt , MyGeom , MyBC , MyRhsFun ) 

elif PROBLEM == "CONVECTION-DIFFUSION" :

    SOL = solve2D_convection_diffusion( MyElt , MyGeom , MyBC , MyRhsFun )

elif PROBLEM == "STOKES" :

    (U,V,P) = Py_solve2D_stokes( MyElt , MyEltM1 , MyGeom , MyBC , MyRhsFun )
	
elif PROBLEM == "NAVIER-STOKES" :

    (U,V,P) = Py_solve2D_NavierStokes( MyElt , MyEltM1 , MyGeom , MyBC , MyRhsFun )

else:

    print("Problem \"%s\" not yet implemented" % PROBLEM)
    exit(EXIT_FAILURE)
   
   
#----------------------- graphics output -----------------------------

if   PROBLEM == "LAPLACIAN" :

    graphics2D( "vtk" , MyElt , MyGeom , SOL , "Sol_Approch")

elif PROBLEM == "CONVECTION-DIFFUSION" :

    graphics2D( "vtk" , MyElt , MyGeom , SOL , "Sol_Approch")

elif PROBLEM == "STOKES" :

    graphics2D_stokes("vtk" , MyElt , MyGeom , U,V,P, "Stokes")
    graphics2D_stokes("silo", MyElt , MyGeom , U,V,P, "Stokes")

elif PROBLEM == "NAVIER-STOKES" :

    graphics2D( "vtk" , MyElt , MyGeom , U , "NavierStokes_U_")
    graphics2D( "vtk" , MyElt , MyGeom , V , "NavierStokes_V_")
    graphics2D( "vtk" , MyEltM1, GEOM_2D_geomBase_get(MyGeom), P, "NavierStokes_P_")
    graphics2D( "silo", MyEltM1, GEOM_2D_geomBase_get(MyGeom), P, "NavierStokes_P_")

    graphics2D_stokes("vtk" , MyElt , MyGeom , U,V,P, "NavierStokes")
    graphics2D_stokes("silo", MyElt , MyGeom , U,V,P, "NavierStokes")

else:

    print("Problem \"%s\" not yet implemented" % PROBLEM)
    exit(EXIT_FAILURE)

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

mem_info_file(stdout,0)
   
mem_info_file(stdout,MY_LIST1)
mem_info_file(stdout,MY_LIST2) 
mem_info_file(stdout,MY_LIST3) 
mem_info_file(stdout,MY_LIST4) 
mem_info_file(stdout,MY_LIST5) 
mem_info_file(stdout,MY_LIST6)
mem_info_file(stdout,MY_LIST7)

#----------------------------------------------------------------------
