#!/usr/bin/env gnuplot
#
# Domaine d'interpolation :
#
set dummy t
set grid
set size square
#
#

x0= -2
Fx0(t) = (t>x0)?(t-x0)**2:0
#
x1= -1
Fx1(t) = (t>x1)?(t-x1)**2:0
#
x2= 0
Fx2(t) = (t>x2)?(t-x2)**2:0
#
x3= 1
Fx3(t) = (t>x3)?(t-x3)**2:0
#
#
D3F(t) =  Fx0(t) - 3*Fx1(t) + 3*Fx2(t) - Fx3(t)
plot [-4:3][-1:4] D3F(t)
pause -1
#
#
# ##################################
#       DEF of SPLINE :            #
# ##################################
h=1
#
#
# ###############################
# Many splines
# ###############################
#
h = 1
#
F1(x,t) = (t-x)**2
F2(x,t) = h**2 + 2*h*(t-x-1) - 2*h*(t-x-1)**2
F3(x,t) = (x+3-t)**2
#
# 
spline1(x,t) = (t>x)? \
              (t>x+1)? \
                (t>x+2)? \
                  (t>x+3)? \
                   0:F3(x,t):F2(x,t):F1(x,t):0
#
# peridicity
splineR(x,t)  = (t>x-4)? \
                 (t>x-3)? \
                  (t>x-2)? \
                   (t>x-1)? \
                    (t>x)? \
                     (t>x+1)? \
                      (t>x+2)? \
                  0:F2(x,t):F1(x,t):0:0:0:F1(x-3,t):0
#
# peridicity
splineL(x,t) =  (t>x)? \
                 (t>x+1)? \
                  (t>x+2)? \
                   (t>x+3)? \
                    (t>x+4)? \
                     (t>x+5)? \
                      (t>x+6)? \
                       (t>x+7)? \
                   0:F3(x+3,t):0:0:0:F3(x,t):F2(x,t):0:0
#
#
plot [0:6][-1:4] \
  splineL(-1,t),spline1(0,t),spline1(1,t),spline1(2,t),spline1(3,t),splineR(4,t), \
  splineL(-1,t)+spline1(0,t)+spline1(1,t)+spline1(2,t)+spline1(3,t)+splineR(4,t), \
  2*splineL(-1,t)+0*spline1(0,t)+2*spline1(1,t)+0*spline1(2,t)+2*spline1(3,t)+0*splineR(4,t)   
              
pause -1

#   Real MAT_PermBasis_S2[2][2] =     /* SUMM_j (A_ij) = 0 */
#
#   {   1.0/2 ,   1.0/2 ,
#       1.0/4 ,  -1.0/4 } ;
#

SPLINE0(t) = spline1(-2,t)
SPLINE1(t) = spline1(-1,t)

a00 = 1.0/2
a01 = 1.0/2
#
a10 = -1.0/4
a11 =  1.0/4
#

NEW_SPLINE0(t) = a00*SPLINE0(t) + a01*SPLINE1(t)
NEW_SPLINE1(t) = a10*SPLINE0(t) + a11*SPLINE1(t)

plot [-2:2][-1:3] \
   SPLINE0(t) , \
   SPLINE1(t) , \
   NEW_SPLINE0(t), \
   NEW_SPLINE1(t)
pause -1
