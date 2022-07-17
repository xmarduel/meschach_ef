#!/usr/local/bin/gnuplot
#
# B-Splines order 5 :
#
set dummy t
set grid
#
#

x0= -3
Fx0(t) = (t>x0)?(t-x0)**5:0
#
x1= -2
Fx1(t) = (t>x1)?(t-x1)**5:0
#
x2= -1
Fx2(t) = (t>x2)?(t-x2)**5:0
#
x3=  0
Fx3(t) = (t>x3)?(t-x3)**5:0
#
x4=  1
Fx4(t) = (t>x4)?(t-x4)**5:0
#
x5=  2
Fx5(t) = (t>x5)?(t-x5)**5:0
#
x6=  3
Fx6(t) = (t>x6)?(t-x6)**5:0
#
#
#
plot [-5:5][-1:5] \
   Fx0(t) , \
   Fx1(t) , \
   Fx2(t) , \
   Fx3(t) , \
   Fx4(t) , \
   Fx5(t) , \
   Fx6(t) 
pause -1
#
#
D6F(t) =  Fx6(t) - 6*Fx5(t) + 15*Fx4(t) - 20*Fx3(t) + \
             15*Fx2(t) - 6*Fx1(t) + Fx0(t)
plot [-5:5][-1:80] D6F(t)
pause -1
#
#
# ##################################
#       DEF of SPLINE :            #
# ##################################
h=1
#
F1(t) = Fx0(t)
F2(t) = Fx0(t) - 6*Fx1(t)
F3(t) = Fx0(t) - 6*Fx1(t) + 15*Fx2(t)
F4(t) = Fx0(t) - 6*Fx1(t) + 15*Fx2(t) - 20*Fx3(t)
F5(t) = Fx0(t) - 6*Fx1(t) + 15*Fx2(t) - 20*Fx3(t) + 15*Fx4(t)
F6(t) = Fx0(t) - 6*Fx1(t) + 15*Fx2(t) - 20*Fx3(t) + 15*Fx4(t) - 6*Fx5(t)
#
spline1(t) = (t>x0)? \
               (t>x1)? \
                 (t>x2)? \
                   (t>x3)? \
                     (t>x4)? \
                       (t>x5)? \
                         (t>x6)? \
             0:F6(t):F5(t):F4(t):F3(t):F2(t):F1(t):0
plot [-4:4] spline1(t)
pause -1
#
#
#
G1(t) = (t-x0)**5
G2(t) = - 5*(t-x1)**5 +  5*(h**1)*(t-x1)**4 + \
                        10*(h**2)*(t-x1)**3 + \
                        10*(h**3)*(t-x1)**2 + \
                         5*(h**4)*(t-x1)**1 + \
                         1*(h**5)*(t-x1)**0
G3(t) = 10*(t-x2)**5 -  20*(h**1)*(t-x2)**4 - \
                        20*(h**2)*(t-x2)**3 + \
                        20*(h**3)*(t-x2)**2 + \
                        50*(h**4)*(t-x2)**1 + \
                        26*(h**5)*(t-x2)**0
G4(t) = 10*(x4-t)**5 -  20*(h**1)*(x4-t)**4 - \
                        20*(h**2)*(x4-t)**3 + \
                        20*(h**3)*(x4-t)**2 + \
                        50*(h**4)*(x4-t)**1 + \
                        26*(h**5)*(x4-t)**0
G5(t) =  - 5*(x5-t)**5 + 5*(h**1)*(x5-t)**4 + \
                        10*(h**2)*(x5-t)**3 + \
                        10*(h**3)*(x5-t)**2 + \
                         5*(h**4)*(x5-t)**1 + \
                         1*(h**5)*(x5-t)**0
G6(t) = (x6-t)**5
#
spline2(t) = (t>x0)? \
               (t>x1)? \
                 (t>x2)? \
                   (t>x3)? \
                     (t>x4)? \
                       (t>x5)? \
                         (t>x6)? \
             0:G6(t):G5(t):G4(t):G3(t):G2(t):G1(t):0
plot [-4:4]  spline1(t) with lines , \
             spline2(t) with points
pause -1
#
#
plot [-4:4] spline1(t) with lines , \
            D6F(t) with points
pause -1
#
#
B1(t) = (1-t)**5
B2(t) = - 5*(1-t)**5 +  5*(1-t)**4 + 10*(1-t)**3 + 10*(1-t)**2 +  5*(1-t) +  1
B3(t) =  10*(1-t)**5 - 20*(1-t)**4 - 20*(1-t)**3 + 20*(1-t)**2 + 50*(1-t) + 26
B4(t) =  10*t**5 - 20*t**4 - 20*t**3 + 20*t**2 + 50*t + 26
B5(t) = - 5*t**5 +  5*t**4 + 10*t**3 + 10*t**2 +  5*t +  1
B6(t) =     t**5
# 
plot [0:1] B1(t), \
           B2(t), \
           B3(t), \
           B4(t), \
           B5(t), \
           B6(t)
pause -1
#
#                 
plot [0:1] B1(t), \
           B2(t), \
           B3(t), \
           B4(t), \
           B5(t), \
           B6(t), \
	   B1(t) + B2(t) + B3(t) + B4(t) + B5(t) + B6(t)
pause -1
#
#
#
B0(t) = spline2(t+2)
B1(t) = spline2(t+1)
B2(t) = spline2(t)
B3(t) = spline2(t-1)
B4(t) = spline2(t-2)
#
plot [-5.00:5.00][-1.0:100.0] \
	B0(t)    title "B0(t)" with lines ,\
	B1(t)    title "B1(t)" with lines ,\
	B2(t)    title "B2(t)" with lines ,\
	B3(t)    title "B3(t)" with lines ,\
	B4(t)    title "B4(t)" with lines
pause -1
#
#
set size square
#
a00 = -1.0/60 
a01 = 7.0/480
a02 = 1.0/240
a03 =  7.0/480
a04 = -1.0/60
#
a10 = -1.0/240 
a11 = 13.0/1920
a12 = -1.0/192
a13 = 13.0/1929
a14 = -1.0/240
#
a20 = -1.0/60
a21 = -1.0/120
a22 = 0.0
a23 = 1.0/120
a24 = 1.0/60
#
a30 = -1.0/144
a31 =  1.0/1440
a32 =  0.0
a33 =  -1.0/1440
a34 =  1.0/144
#
a40 = 3.0/50/2
a41 = -3.0/200/2
a42 = 1.0/100/2
a43 = -3.0/200/2
a44 = 3.0/50/2
#
#
BB0(t) = a00*B0(t) + a01*B1(t) + a02*B2(t) + a03*B3(t) + a04*B4(t)
BB1(t) = a10*B0(t) + a11*B1(t) + a12*B2(t) + a13*B3(t) + a14*B4(t)
BB2(t) = a20*B0(t) + a21*B1(t) + a22*B2(t) + a23*B3(t) + a24*B4(t)
BB3(t) = a30*B0(t) + a31*B1(t) + a32*B2(t) + a33*B3(t) + a34*B4(t)
BB4(t) = a40*B0(t) + a41*B1(t) + a42*B2(t) + a43*B3(t) + a44*B4(t)
#
plot [-5.0:5.0][-3.0:5.0] \
	BB0(t)    title "BB0(t)"  with lines ,\
	BB1(t)    title "BB1(t)"  with lines ,\
	BB2(t)    title "BB2(t)"  with lines ,\
	BB3(t)    title "BB3(t)"  with lines ,\
	BB4(t)    title "BB4(t)"  with lines
pause -1
#
