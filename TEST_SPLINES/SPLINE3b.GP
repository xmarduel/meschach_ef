#!/usr/local/bin/gnuplot
#
# Domaine d'interpolation :
#
set dummy t
set grid
#
#set noautoscale
#
set size square
#
#
x0= -2
Fx0(t) = (t>x0)?(t-x0)**3:0
#
x1= -1
Fx1(t) = (t>x1)?(t-x1)**3:0
#
x2= 0
Fx2(t) = (t>x2)?(t-x2)**3:0
#
x3= 1
Fx3(t) = (t>x3)?(t-x3)**3:0
#
x4= 2
Fx4(t) = (t>x4)?(t-x4)**3:0
#
#
#
pause 0 "Some C-2 functions "
plot [-5:5][-1:5] \
   Fx0(t) , \
   Fx1(t) , \
   Fx2(t) , \
   Fx3(t) , \
   Fx4(t)
pause -1
#
#
pause 0 "Let's take a good linear combinaisons"
pause 0 "of these 5 functions : the result is ..."
D4F(t) =  Fx4(t) - 4*Fx3(t) + 6*Fx2(t) - 4*Fx1(t) + Fx0(t)
plot [-5:5][-1:5] D4F(t)
pause -1
#
#
# ##################################
#       DEF of SPLINE :            #
# ##################################
h=1
#
F1(t) = (t-x0)**3
F2(t) = h**3 + 3*(h**2)*(t-x1) + 3*h*(t-x1)**2 - 3*(t-x1)**3
F3(t) = h**3 + 3*(h**2)*(x3-t) + 3*h*(x3-t)**2 - 3*(x3-t)**3
F4(t) = (x4-t)**3
# 
spline(t) = (t>x0)? \
              (t>x1)? \
                (t>x2)? \
                  (t>x3)? \
                    (t>x4)? \
                   0:F4(t):F3(t):F2(t):F1(t):0
pause 0 "I define a function with a formula :"
pause 0 "on all interval [x_i,x_i+1], spline(t) is given"
plot [-3:3] spline(t)
pause -1
#
#
pause 0 "Well, let s compare spline(t) and the linear combinaison D4F(t)"
pause 0 "It should be the same ..."
plot [-3:3] spline(t) with lines , \
            D4F(t) with points
pause -1
#
#
BASE1(t) = (1-t)**3
BASE2(t) =  1 + 3*(1-t) + 3*(1-t)**2 - 3*(1-t)**3 
BASE3(t) =  1 + 3*(t-0) + 3*(t-0)**2 - 3*(t-0)**3
BASE4(t) =  t**3
# 
pause 0 "the 4 basis functions for finite element"
pause 0 "on the reference interval"
plot [0:1] BASE1(t), \
           BASE2(t), \
           BASE3(t), \
           BASE4(t)
pause -1
#
#                 
# On somme les 4 fonctions de base
SOMME(t) = BASE1(t) + BASE2(t) + BASE3(t) + BASE4(t)
#
pause 0 "By the way, the sum of these 4 functions is a constant"
plot [0:1] BASE1(t), \
           BASE2(t), \
           BASE3(t), \
           BASE4(t), \
	   SOMME(t) 
pause -1
#
#
SPLINE0(t) = spline(t+1)
SPLINE1(t) = spline(t)
SPLINE2(t) = spline(t-1)
#
plot [-3:3][-1:5] SPLINE0(t) title "spline0(t)" with lines ,\
                  SPLINE1(t) title "spline1(t)" with lines ,\
                  SPLINE2(t) title "spline2(t)" with lines
#
pause -1
#
# Linear Combinaisons of SPLINE0(t), SPLINE1(t) and SPLINE2(t) :
#
#   Real MAT_PermBasis_S3_a[3][3] =     /* A_ii = 1 */
#
#   {   1.0   ,  -7.0/12,  4.0/3 ,   /* TildePhi_0 =      Phi_0 - 7/12*Phi_1 + 4/3*Phi_2 */
#      -3.0/2 ,   1.0   , -3.0/2 ,   /* TildePhi_1 = -3/2*Phi_0 +      Phi_1 - 3/2*Phi_2 */
#       1.0   ,  -1.0/2 ,  1.0   } ; /* TildePhi_2 =      Phi_0 - 1/02*Phi_1 +     Phi_2 */
#
#
#   Real MAT_PermBasis_S3_b[3][3] =     /* SUMM_j (A_ij) = 1 */
#
#   {   1.0/2 ,  -1.0/3 ,  5.0/6 ,
#       1.0/2 ,   0.0   ,  1.0/2 ,
#       2.0/3 ,  -1.0/3 ,  2.0/3 } ;
#
#
#   Real MAT_PermBasis_S3_c[3][3] =     /* SUMM_j (A_ij) = 0 */
#
#   {  -1.0/6 ,   0.0   ,  1.0/6 ,
#      -1.0/6 ,   1.0/3 , -1.0/6 ,
#       2.0/3 ,  -0.0/3 , -2.0/3 } ;
#
# --------------------------------------------- 
#
#
pause 0 "Linear combinaison of B_-1, B_o and B_1:"
pause 0 "-----------------------------------------------------------------"
pause 0 "NEW_SPLINE0(t) = a00*SPLINE0(t) + a01*SPLINE1(t) + a02*SPLINE2(t)"
pause 0 "NEW_SPLINE1(t) = a10*SPLINE0(t) + a11*SPLINE1(t) + a12*SPLINE2(t)"
pause 0 "NEW_SPLINE2(t) = a20*SPLINE0(t) + a21*SPLINE1(t) + a22*SPLINE2(t)"
pause 0 "-----------------------------------------------------------------"
pause 0 "   [   1.0   ,  -7.0/12,  4.0/3 ]"
pause 0 "   [  -3.0/2 ,   1.0   , -3.0/2 ]"
pause 0 "   [    1.0  ,  -1.0/2 ,  1.0   ]"
pause 0 "-----------------------------------------------------------------"
pause 0 " A_ii = 1 
pause 0 "-----------------------------------------------------------------"
pause 0 "NEW_SPLINE0(t) satisfies ZERO boundary condition -- derivee = 1"
pause 0 "NEW_SPLINE1(t) satisfies ONE  boundary condition -- derivee = 0"
pause 0 "NEW_SPLINE2(t) satisfies ZERO boundary condition -- derivee = 0"
pause 0 "-----------------------------------------------------------------"
#
#
pause 0 "satisfy ZERO boundary condition -- derivee = 1"
pause 0 " "
pause 0 "Linear combinaison of B_-1, B_o and B_1: Btilde_o = 0.25*B_o"
pause 0 "satisfy ZERO boundary condition -- derivee = 0"
pause 0 " "
pause 0 "Linear combinaison of B_-1, B_o and B_1: Btilde_1 = B_-1 - 1/2*B_o + B_1"
pause 0 "satisfy ZERO boundary condition -- derivee = 0"
pause 0 " "
#
#
# Real MAT_PermBasis_S3_a
#
a00 = 1.0
a01 = -7.0/12
a02 = 4.0/3
#
a10 = -3.0/2
a11 = 1.0
a12 = -3.0/2
#
a20 = 1.0
a21 = -1.0/2
a22 = 1.0
#
NEW_SPLINE0(t) = a00*SPLINE0(t) + a01*SPLINE1(t) + a02*SPLINE2(t)
NEW_SPLINE1(t) = a10*SPLINE0(t) + a11*SPLINE1(t) + a12*SPLINE2(t)
NEW_SPLINE2(t) = a20*SPLINE0(t) + a21*SPLINE1(t) + a22*SPLINE2(t)
#
#NEW_SPLINE0(t) =   0.25*SPLINE1(t) + 0.0*SPLINE2(t)
#NEW_SPLINE1(t) =   0.25*SPLINE1(t) + 0.0*SPLINE2(t)
#NEW_SPLINE2(t) =  -0.25*SPLINE1(t) + 1.0*SPLINE2(t)
#
set size square
#
plot [-3.03:3.03][-1.01:5.05] \
	NEW_SPLINE0(t)   title "NEW_SPLINE0(t)" with lines ,\
	NEW_SPLINE1(t)   title "NEW_SPLINE1(t)" with lines ,\
	NEW_SPLINE2(t)   title "NEW_SPLINE2(t)" with lines 
#	SPLINE0(t)   title "SPLINE0(t)"           with lines ,\
#	SPLINE1(t)   title "SPLINE1(t)"           with lines ,\
#	SPLINE2(t)   title "SPLINE2(t)"           with lines
pause -1
#
#----------------------------------------------
#Real MAT_PermBasis_S3_b
#
a00 =  1.0/2
a01 = -1.0/3
a02 =  5.0/6
#
a10 =  1.0/2
a11 =  0.0
a12 =  1.0/2
#
a20 =  2.0/3
a21 = -1.0/3
a22 =  2.0/3
#
#
NEW_SPLINE0(t) = a00*SPLINE0(t) + a01*SPLINE1(t) + a02*SPLINE2(t)
NEW_SPLINE1(t) = a10*SPLINE0(t) + a11*SPLINE1(t) + a12*SPLINE2(t)
NEW_SPLINE2(t) = a20*SPLINE0(t) + a21*SPLINE1(t) + a22*SPLINE2(t)
#
set size square
#
pause 0 "Linear combinaison of B_-1, B_o and B_1:"
pause 0 "-----------------------------------------------------------------"
pause 0 "NEW_SPLINE0(t) = a00*SPLINE0(t) + a01*SPLINE1(t) + a02*SPLINE2(t)"
pause 0 "NEW_SPLINE1(t) = a10*SPLINE0(t) + a11*SPLINE1(t) + a12*SPLINE2(t)"
pause 0 "NEW_SPLINE2(t) = a20*SPLINE0(t) + a21*SPLINE1(t) + a22*SPLINE2(t)"
pause 0 "-----------------------------------------------------------------"
pause 0 "   [   1.0/2 ,  -1.0/3 ,  5.0/6 ]"
pause 0 "   [   1.0/2 ,   0.0   ,  1.0/2 ]"
pause 0 "   [   2.0/3 ,  -1.0/3 ,  2.0/3 ]"
pause 0 "-----------------------------------------------------------------"
pause 0 " A_ii = 1 "
pause 0 "-----------------------------------------------------------------"
pause 0 "NEW_SPLINE0(t) satisfies ZERO boundary condition -- derivee = 1"
pause 0 "NEW_SPLINE1(t) satisfies ONE  boundary condition -- derivee = 0"
pause 0 "NEW_SPLINE2(t) satisfies ZERO boundary condition -- derivee = 0"
pause 0 "-----------------------------------------------------------------"
#
plot [-3.03:3.03][-1.01:5.05] \
	NEW_SPLINE0(t)   title "NEW_SPLINE0(t)" with lines ,\
	NEW_SPLINE1(t)   title "NEW_SPLINE1(t)" with lines ,\
	NEW_SPLINE2(t)   title "NEW_SPLINE2(t)" with lines 
#	SPLINE0(t)   title "SPLINE0(t)"           with lines ,\
#	SPLINE1(t)   title "SPLINE1(t)"           with lines ,\
#	SPLINE2(t)   title "SPLINE2(t)"           with lines
pause -1
#
#---------------------------------------
#Real MAT_PermBasis_S3_c
#
a00 = -1.0/6
a01 =  0.0
a02 =  1.0/6
#
a10 = -1.0/6
a11 =  1.0/3
a12 = -1.0/6
#
a20 =   2.0/8
a21 =  -1.0/8
a22 =   2.0/8
#
#
NEW_SPLINE0(t) = a00*SPLINE0(t) + a01*SPLINE1(t) + a02*SPLINE2(t)
NEW_SPLINE1(t) = a10*SPLINE0(t) + a11*SPLINE1(t) + a12*SPLINE2(t)
NEW_SPLINE2(t) = a20*SPLINE0(t) + a21*SPLINE1(t) + a22*SPLINE2(t)
#
set size square
#
pause 0 "Linear combinaison of B_-1, B_o and B_1:"
pause 0 "-----------------------------------------------------------------"
pause 0 "NEW_SPLINE0(t) = a00*SPLINE0(t) + a01*SPLINE1(t) + a02*SPLINE2(t)"
pause 0 "NEW_SPLINE1(t) = a10*SPLINE0(t) + a11*SPLINE1(t) + a12*SPLINE2(t)"
pause 0 "NEW_SPLINE2(t) = a20*SPLINE0(t) + a21*SPLINE1(t) + a22*SPLINE2(t)"
pause 0 "-----------------------------------------------------------------"
pause 0 "   [  -1.0/6 ,   0.0   ,  1.0/6 ]"
pause 0 "   [  -1.0/6 ,   1.0/3 , -1.0/6 ]"
pause 0 "   [   2.0/3 ,  -0.0/3 , -2.0/3 ]"
pause 0 "-----------------------------------------------------------------"
pause 0 " SUMM_j (A_ij) = 0 "
pause 0 "-----------------------------------------------------------------"
pause 0 "NEW_SPLINE0(t) satisfies ZERO boundary condition -- derivee = 1"
pause 0 "NEW_SPLINE1(t) satisfies ONE  boundary condition -- derivee = 0"
pause 0 "NEW_SPLINE2(t) satisfies ZERO boundary condition -- derivee = 0"
pause 0 "-----------------------------------------------------------------"
#
plot [-3.03:3.03][-1.01:5.05] \
	NEW_SPLINE0(t)   title "NEW_SPLINE0(t)" with lines ,\
	NEW_SPLINE1(t)   title "NEW_SPLINE1(t)" with lines ,\
	NEW_SPLINE2(t)   title "NEW_SPLINE2(t)" with lines
#	SPLINE0(t)   title "SPLINE0(t)"           with lines ,\
#	SPLINE1(t)   title "SPLINE1(t)"           with lines ,\
#	SPLINE2(t)   title "SPLINE2(t)"           with lines
pause -1
#
