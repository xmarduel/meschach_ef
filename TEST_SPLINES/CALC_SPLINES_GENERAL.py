#! /usr/bin/env python
#
#
from sys import *
from math import *
#
# General Splines :  with bc y'(X0) and y'(X4) given 
dy0 = 0.0
dy4 = 0.0
#
from Numeric import *
from LinearAlgebra import *
#
#-------------------------------------------------------------------
# store the coeff on each interval
coeffs = zeros((4,4),Float) # 4 coeffs on 4 intervals
#
# the mesh
mesh = zeros(4,Float)
#
# for the resolution : eval y_xx[0:4] <- sol
mat  = zeros((5,5),Float)
vec  = zeros((5,1),Float)
sol  = zeros((5,1),Float)
#
y_xx  = zeros(5,Float)
y     = zeros(5,Float)
#
# the points to interpolate
y[0] = 0.0
y[1] = 1.0
y[2] = 4.0
y[3] = 1.0
y[4] = 0.0
#
# the mesh values
mesh[0] = 2.0
mesh[1] = 2.0
mesh[2] = 1.0
mesh[3] = 4.0
#
print("mesh = \n", mesh)
#
#
mat[0][0] = 2*(mesh[0]) 
mat[0][1] =           (mesh[0]) 
#
mat[1][0] =   (mesh[0]) 
mat[1][1] = 2*(mesh[0]+mesh[1]) 
mat[1][2] =           (mesh[1]) 
#
mat[2][1] =   (mesh[1]) 
mat[2][2] = 2*(mesh[1]+mesh[2]) 
mat[2][3] =           (mesh[2]) 
#
mat[3][2] =   (mesh[2]) 
mat[3][3] = 2*(mesh[2]+mesh[3]) 
mat[3][4] =           (mesh[3]) 
#
mat[4][3] =   (mesh[3]) 
mat[4][4] = 2*(mesh[3]) 
#
#
print("mat = \n", mat)
#
vec[0] = -6.0*dy0                   + 6.0/mesh[0]*( y[1]-y[0] ) 
vec[1] = -6.0/mesh[0]*( y[1]-y[0] ) + 6.0/mesh[1]*( y[2]-y[1] )
vec[2] = -6.0/mesh[1]*( y[2]-y[1] ) + 6.0/mesh[2]*( y[3]-y[2] )
vec[3] = -6.0/mesh[2]*( y[3]-y[2] ) + 6.0/mesh[3]*( y[4]-y[3] )
vec[4] = -6.0/mesh[3]*( y[4]-y[3] ) + 6.0*dy4 
#
#
# solve and find y_xx[1], y_xx[2], y_xx[3]
#
sol = solve_linear_equations(mat,vec)
#
print("verify mat*sol = vec :")
print(" ## mat*sol = ")
print(matrixmultiply(mat, sol))
print(" ## vec = ")
print(vec)
#
y_xx[0] = sol[0][0]
y_xx[1] = sol[1][0]
y_xx[2] = sol[2][0]
y_xx[3] = sol[3][0]
y_xx[4] = sol[4][0]
#
#
# get the coeff of the function
#
for i in range(4):
    #
    print(" i = ",i)
    coeffs[i][0] = 1.0/(6.0*mesh[i]) * ( y_xx[i+1] - y_xx[i] )
    coeffs[i][1] = 0.5 * y_xx[i]
    coeffs[i][2] = 1.0/(mesh[i]) * ( y[i+1] - y[i] ) - (1.0/6.0)*mesh[i] * ( y_xx[i+1] + 2*y_xx[i] )
    coeffs[i][3] = y[i]
    #
#
#
X0 = 0.0
X1 = X0 + mesh[0]
X2 = X1 + mesh[1]
X3 = X2 + mesh[2]
X4 = X3 + mesh[3]
#
#
# for gnuplot
#
print("X0 = ", X0)
print("X1 = ", X1)
print("X2 = ", X2)
print("X3 = ", X3)
print("X4 = ", X4)
#
print("F1(x) = ", coeffs[0][0],"*(x-X0)**3 + ", coeffs[0][1],"*(x-X0)**2 + ", coeffs[0][2],"*(x-X0) + ", coeffs[0][3])
print("F2(x) = ", coeffs[1][0],"*(x-X1)**3 + ", coeffs[1][1],"*(x-X1)**2 + ", coeffs[1][2],"*(x-X1) + ", coeffs[1][3])
print("F3(x) = ", coeffs[2][0],"*(x-X2)**3 + ", coeffs[2][1],"*(x-X2)**2 + ", coeffs[2][2],"*(x-X2) + ", coeffs[2][3])
print("F4(x) = ", coeffs[3][0],"*(x-X3)**3 + ", coeffs[3][1],"*(x-X3)**2 + ", coeffs[3][2],"*(x-X3) + ", coeffs[3][3])
#
print("DF1(x) = ", 3*coeffs[0][0],"*(x-X0)**2 + ", 2*coeffs[0][1],"*(x-X0)**1 + ", coeffs[0][2] )
print("DF2(x) = ", 3*coeffs[1][0],"*(x-X1)**2 + ", 2*coeffs[1][1],"*(x-X1)**1 + ", coeffs[1][2] )
print("DF3(x) = ", 3*coeffs[2][0],"*(x-X2)**2 + ", 2*coeffs[2][1],"*(x-X2)**1 + ", coeffs[2][2])
print("DF4(x) = ", 3*coeffs[3][0],"*(x-X3)**2 + ", 2*coeffs[3][1],"*(x-X3)**1 + ", coeffs[3][2])
#
print("DDF1(x) = ", 6*coeffs[0][0],"*(x-X0)**1 + ", 2*coeffs[0][1])
print("DDF2(x) = ", 6*coeffs[1][0],"*(x-X1)**1 + ", 2*coeffs[1][1])
print("DDF3(x) = ", 6*coeffs[2][0],"*(x-X2)**1 + ", 2*coeffs[2][1])
print("DDF4(x) = ", 6*coeffs[3][0],"*(x-X3)**1 + ", 2*coeffs[3][1])
#
print("dy0    = ", coeffs[0][2])
print("dy4    = ", 3*coeffs[3][0],"*(X4-X3)**2 + ", 2*coeffs[3][1],"*(X4-X3) + ", coeffs[3][2])
print("ddy4   = ", 6*coeffs[3][0],"*(X4-X3)    + ", 2*coeffs[3][1])
#
#
print("X0_m_1 = X0 - 1")
print("X4_p_1 = X4 + 1")
#
print("spline(x) = (x>X0)? (x>X1)? (x>X2)? (x>X3)? (x>X4)? 0:F4(x):F3(x):F2(x):F1(x):0" )
print("plot [X0_m_1:X4_p_1] spline(x)")
print("pause -1")
#
print("Dspline(x) = (x>X0)? (x>X1)? (x>X2)? (x>X3)? (x>X4)? 0:DF4(x):DF3(x):DF2(x):DF1(x):0" )
print("plot [X0_m_1:X4_p_1] Dspline(x)")
print("pause -1")
#
print("DDspline(x) = (x>X0)? (x>X1)? (x>X2)? (x>X3)? (x>X4)? 0:DDF4(x):DDF3(x):DDF2(x):DDF1(x):0" )
print("plot [X0_m_1:X4_p_1] DDspline(x)")
print("pause -1")
#
#