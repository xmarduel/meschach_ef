#!/usr/local/bin/gnuplot
#
# B-Splines order 4 :
#
set dummy t
set grid
#
#

x0= -3
Fx0(t) = (t>x0)?(t-x0)**4:0
#
x1= -2
Fx1(t) = (t>x1)?(t-x1)**4:0
#
x2= -1
Fx2(t) = (t>x2)?(t-x2)**4:0
#
x3=  0
Fx3(t) = (t>x3)?(t-x3)**4:0
#
x4=  1
Fx4(t) = (t>x4)?(t-x4)**4:0
#
x5=  2
Fx5(t) = (t>x5)?(t-x5)**4:0
#
#
#
plot [-5:5][-1:5] \
   Fx0(t) , \
   Fx1(t) , \
   Fx2(t) , \
   Fx3(t) , \
   Fx4(t) , \
   Fx5(t)
pause -1
#
#
D5F(t) =  Fx0(t) - 5*Fx1(t) + 10*Fx2(t) - 10*Fx3(t) + \
             5*Fx4(t) - Fx5(t)
plot [-5:4][-1:20] D5F(t)
pause -1
#
#
# ##################################
#       DEF of SPLINE :            #
# ##################################
h=1
#
F1(t) = Fx0(t)
F2(t) = Fx0(t) - 5*Fx1(t)
F3(t) = Fx0(t) - 5*Fx1(t) + 10*Fx2(t)
F4(t) = Fx0(t) - 5*Fx1(t) + 10*Fx2(t) - 10*Fx3(t)
F5(t) = Fx0(t) - 5*Fx1(t) + 10*Fx2(t) - 10*Fx3(t) + 5*Fx4(t)
#
spline1(t) = (t>x0)? \
               (t>x1)? \
                 (t>x2)? \
                   (t>x3)? \
                     (t>x4)? \
                       (t>x5)? \
             0:F5(t):F4(t):F3(t):F2(t):F1(t):0
plot [-4:4] spline1(t)
pause -1
#
#
#
G1(t) = (t-x0)**4
G2(t) = -  4*(t-x1)**4 \
        +  4*(t-x1)**3 \
	+  6*(t-x1)**2 \
	+  4*(t-x1)**1 \
	+  1*(t-x1)**0
G3(t) =    6*(t-x2)**4 \
        - 12*(t-x2)**3 \
        -  6*(t-x2)**2 \
        + 12*(t-x2)**1 \
	+ 11
G4(t) = -  4*(x4-t)**4 \
        +  4*(x4-t)**3 \
        +  6*(x4-t)**2 \
        +  4*(x4-t)**1 \
        +  1*(x4-t)**0
G5(t) =      (x5-t)**4
#
spline2(t) = (t>x0)? \
               (t>x1)? \
                 (t>x2)? \
                   (t>x3)? \
                     (t>x4)? \
                       (t>x5)? \
             0:G5(t):G4(t):G3(t):G2(t):G1(t):0
plot [-4:4]  spline1(t) with lines , \
             spline2(t) with points
pause -1
#
#
plot [-4:4] spline1(t) with lines , \
            D5F(t) with points
pause -1
#
#
B1(t) =    (1-t)**4
B2(t) = -4*(1-t)**4 +  4*(1-t)**3 + 6*(1-t)**2 + 4*(1-t)**1 + 1
B3(t) =  6*t**4 - 12*t**3 - 6*t**2 + 12*t**1 + 11
B4(t) = -4*t**4 +  4*t**3 + 6*t**2 +  4*t**1 + 1
B5(t) =    t**4
# 
plot [0:1] B1(t), \
           B2(t), \
           B3(t), \
           B4(t), \
           B5(t), \
	   B1(t) + B2(t) + B3(t) + B4(t) + B5(t)
pause -1                  