#
#
from libmeschach import *
from libmeschach_adds import *
from libmeschach_ef import *
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
def fbase_P3_0(X,Y):
    return 1.0 - 5.5*X - 5.5*Y +  9.0*X*X + 18.0*X*Y +  9.0*Y*Y -  4.5*X*X*X - 13.5*X*X*Y - 13.5*X*Y*Y -  4.5*Y*Y*Y
def fbase_P3_1(X,Y):
    return   1.0*X             -  4.5*X*X                       +  4.5*X*X*X 
def fbase_P3_2(X,Y):
    return               1.0*Y                       -  4.5*Y*Y                                        +  4.5*Y*Y*Y
def fbase_P3_3(X,Y):
    return   9.0*X         - 22.5*X*X - 22.5*X*Y            + 13.5*X*X*X + 27.0*X*X*Y + 13.5*X*Y*Y 
def fbase_P3_4(X,Y):
    return  -4.5*X         + 18.0*X*X +  4.5*X*Y            - 13.5*X*X*X - 13.5*X*X*Y  
def fbase_P3_5(X,Y):
    return                            -  4.5*X*Y                         + 13.5*X*X*Y  
def fbase_P3_6(X,Y):
    return                            -  4.5*X*Y                                      + 13.5*X*Y*Y
def fbase_P3_7(X,Y):
    return               9.0*Y        - 22.5*X*Y - 22.5*Y*Y              + 13.5*X*X*Y + 27.0*X*Y*Y + 13.5*Y*Y*Y
def fbase_P3_8(X,Y):
    return         - 4.5*Y            +  4.5*X*Y + 18.0*Y*Y                           - 13.5*X*Y*Y - 13.5*Y*Y*Y
def fbase_P3_9(X,Y):
    return                            + 27.0*X*Y                         - 27.0*X*X*Y - 27.0*X*Y*Y
#
baseTriangle = Geom2D_get_base_triangle("P3isoP1", 3) # 3 subdivisions from a initial triangle 
graphics_geom2D_view("X11", baseTriangle, "dummy")
#
elt_P1 = elt2D_get("P1");
#
fun = Fun2D_get();
#
Fun2D_setFunctionPython(fun, fbase_P3_0);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f0_P3_")
Fun2D_setFunctionPython(fun, fbase_P3_1);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f1_P3_")
Fun2D_setFunctionPython(fun, fbase_P3_2);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f2_P3_")
Fun2D_setFunctionPython(fun, fbase_P3_3); 
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f3_P3_") 
Fun2D_setFunctionPython(fun, fbase_P3_4);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f4_P3_")
Fun2D_setFunctionPython(fun, fbase_P3_5);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f5_P3_")
Fun2D_setFunctionPython(fun, fbase_P3_6);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f6_P3_")
Fun2D_setFunctionPython(fun, fbase_P3_7);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f7_P3_")
Fun2D_setFunctionPython(fun, fbase_P3_8);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f8_P3_")
Fun2D_setFunctionPython(fun, fbase_P3_9);
graphics2D_fun( "gnuplot" , elt_P1 , baseTriangle , fun , "Base2D_f9_P3_")
#
#
