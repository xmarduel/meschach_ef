#
#
from libmeschach      import *
from libmeschach_adds import *
from libmeschach_ef   import *
#
#-------------------------------------------------------------------
#
mem_attach_list1();
mem_attach_list2();
mem_attach_list3();
mem_attach_list4();
mem_attach_list5();
mem_attach_list6();
mem_attach_list7();
#
err_warn_attach_lists2();
err_warn_attach_lists3();
err_warn_attach_lists4();
err_warn_attach_lists5();
err_warn_attach_lists6();
err_warn_attach_lists7();
#
set_err_flag(EF_JUMP);
set_warn_flag(EF_SILENT);
#
#
#
#
def fbase_P2_0(x,y):
    return (1.0 - x - y )*( 1.0 -2*x - 2*y)
def fbase_P2_1(x,y):
    return  x*(2*x -1.0 ) 
def fbase_P2_2(x,y):
    return  y*(2*y -1.0)
def fbase_P2_3(x,y):
    return  4*x*(1.0-x-y)
def fbase_P2_4(x,y):
    return  4*x*y  
def fbase_P2_5(x,y):
    return 4*y*(1.0-x-y)  
#
#
baseTriangle = Geom2D_get_base_triangle("P2isoP1",4) # 4 subdivisions from a initial triangle 
graphics_geom2D_view("X11", baseTriangle, "dummy")
#
elt_P1 = elt2D_get("P1");
#
fun = Fun2D_get();
#
Fun2D_setFunctionPython(fun, fbase_P2_0);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f0_P2_")
Fun2D_setFunctionPython(fun, fbase_P2_1);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f1_P2_")
Fun2D_setFunctionPython(fun, fbase_P2_2);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f2_P2_")
Fun2D_setFunctionPython(fun, fbase_P2_3); 
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f3_P2_") 
Fun2D_setFunctionPython(fun, fbase_P2_4);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f4_P2_")
Fun2D_setFunctionPython(fun, fbase_P2_5);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f5_P2_")
#
#
