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


SPLINE0(t) = spline2(t+1)
SPLINE1(t) = spline2(t)
SPLINE2(t) = spline2(t-1)
SPLINE3(t) = spline2(t-2)

set size square
#
plot [-4.03:4.03][-1.0:15.0] \
	SPLINE0(t)   title "SPLINE0(t)" with lines ,\
	SPLINE1(t)   title "SPLINE1(t)" with lines ,\
	SPLINE2(t)   title "SPLINE1(t)" with lines ,\
	SPLINE3(t)   title "SPLINE3(t)" with lines

pause -1
#
#
# how to calculate them ???
#
# F(0) = 1, DF(0) = 0
a00 = 11.0/44
a01 =  1.0/44
a02 =  1.0/44
a03 = 11.0/44
#
# F(0) = 0, DF(0) = 1
a10 =  -11.0/90
a11 =   -1.0/90
a12 =    1.0/90
a13 =   11.0/90
#
# F(0) = 0, DF(0) = 0
# bad ! derivee should be 0 at 0!
#a20 = -11.0/24
#a21 =  11.0/24
#a22 = -11.0/24
#a23 =  11.0/24
#
# F(0) = 0, DF(0) = 0  OK
a20 = -6.0/24.0
a21 =  1.0/12.0
a22 = -1.0/12.0
a23 =  6.0/24.0
#
# F(0) = 0, DF(0) = 0  OK
a30 =  -11.0/22/2
a31 =    1.0/22/2
a32 =    1.0/22/2
a33 =  -11.0/22/2
#
NEW_SPLINE0(t) = a00*SPLINE0(t) + a01*SPLINE1(t) + a02*SPLINE2(t) + a03*SPLINE3(t)
NEW_SPLINE1(t) = a10*SPLINE0(t) + a11*SPLINE1(t) + a12*SPLINE2(t) + a13*SPLINE3(t)
NEW_SPLINE2(t) = a20*SPLINE0(t) + a21*SPLINE1(t) + a22*SPLINE2(t) + a23*SPLINE3(t)
NEW_SPLINE3(t) = a30*SPLINE0(t) + a31*SPLINE1(t) + a32*SPLINE2(t) + a33*SPLINE3(t)
#
set size square
#
plot [-5:5][-5:5] \
	NEW_SPLINE0(t)   title "NEW-SPLINE0(t)" with lines ,\
	NEW_SPLINE1(t)   title "NEW-SPLINE1(t)" with lines ,\
	NEW_SPLINE2(t)   title "NEW-SPLINE2(t)" with lines ,\
	NEW_SPLINE3(t)   title "NEW-SPLINE3(t)" with lines

pause -1
