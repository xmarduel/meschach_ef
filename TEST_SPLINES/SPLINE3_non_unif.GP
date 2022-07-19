#! /usr/bin/env gnuplot
#
# Domaine d'interpolation :
#
set grid
#
#set noautoscale
#
#set size square
#set size ratio -1
#
#
h0 = 0.5
h1 = 1.5
h2 = 1.0
h3 = 2.0
#
#str1 = "h0 = ",h0
#
X0 = 0
X1 = X0 + h0
X2 = X1 + h1
X3 = X2 + h2
X4 = X3 + h3
#
F1(x) =  2.0 *(x-X0)**3 +  0.0 *(x-X0)**2 +  1.11022302463e-16 *(x-X0) +  0.0
F2(x) =  -1.55555555556 *(x-X1)**3 +  3.0 *(x-X1)**2 +  1.5 *(x-X1) +  0.25
F3(x) =  1.77777777778 *(x-X2)**3 +  -4.0 *(x-X2)**2 +  -4.4408920985e-16 *(x-X2) +  4.0
F4(x) =  -0.222222222222 *(x-X3)**3 +  1.33333333333 *(x-X3)**2 +  -2.66666666667 *(x-X3) +  1.77777777778
DF4(x) =  -0.666666666667 *(x-X3)**2 +  2.66666666667 *(x-X3) +  -2.66666666667
dy4    =  -0.666666666667 *(X4-X3)**2 +  2.66666666667 *(X4-X3) +  -2.66666666667
#
print "DF4(X4) = " ,dy4
#
# ##################################
#       DEF of SPLINE :            #
# ##################################
# 
set dummy x

spline(x) = (x>X0)? \
              (x>X1)? \
                (x>X2)? \
                  (x>X3)? \
                    (x>X4)? \
                     0:F4(x):F3(x):F2(x):F1(x):0
pause 0 "I define a function with a formula :"
pause 0 "on all interval [x_i,x_i+1], spline(t) is given"
plot [-1:6] spline(x) title " h0 = 0.5", \
            spline(x) title " h1 = 1.5", \
            spline(x) title " h2 = 1.0", \
            spline(x) title " h3 = 2.0"
pause -1
#
#
#spline(x) = (x>X0)? F1(x):0
#pause 0 "I define a function with a formula :"
#pause 0 "on all interval [x_i,x_i+1], spline(t) is given"
#plot [-1:4] spline(t)
#pause -1