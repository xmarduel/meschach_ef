
import time
import re

from sympy import *

# ---------------------------------------------------------------------------------

x = symbols('x')
y = symbols('y')
z = symbols('z')

# --------------------------------------------------------------------------------

def get_all_monoms(polynom):
    '''
    sympy without this feature ??? 
    
    input :polynom : an "Add" instance
    output: list of monoms ("Add" instances)
    '''
    all_monoms = []

    pp = polys.Poly(polynom, x, y, z)

    coeffs    = pp.coeffs
    exponents = pp.monoms

    for coeff,exponent in zip(coeffs, exponents):
    
        degree_x = exponent[0]
        degree_y = exponent[1]
        degree_z = exponent[2]
    
        m = coeff * (x ** degree_x) * (y ** degree_y) * (z ** degree_z)
    
        all_monoms.append(m)

    return all_monoms


# Integrate...
#
def integrate3D_on_tetrahedra_symbolic(polynom):
    ip_x_   = integrate(polynom, x)
    #print "int on x = ", ip_x_
    ip_x    = ip_x_.subs(x, 1-y-z) - ip_x_.subs(x, 0)
    #print "int on x from 0 to 1-y-z = ", ip_x
    ip_xy_  = integrate(ip_x, y)
    #print "int on y = ", ip_x_y_
    ip_xy   = ip_xy_.subs(y, 1-z) - ip_xy_.subs(y, 0)
    #print "int on y from 0 to 1-z = ", ip_xy
    ip_xyz_ = integrate(ip_xy, z)
    #print "int on z = ", ip_xyz_
    ip_xyz  = ip_xyz_.subs(z, 1) - ip_xyz_.subs(z, 0)
    #print "int on z from 0 to 1 = ", ip_xyz
    return ip_xyz

def fact(n):
    if n == 0:
        return 1
    else:
        return n * fact(n-1)
#
def integrate3D_on_tetrahedra_monomformula(monom):
    '''
    '''
    mm = polys.Poly(monom, x, y, z)

    coeff     = mm.coeffs[0]
    exponent  = mm.monoms[0]

    degree_x = exponent[0]
    degree_y = exponent[1]
    degree_z = exponent[2]

    return coeff * Rational( fact(degree_x) * fact(degree_y) * fact(degree_z) , fact(degree_x + degree_y + degree_z + 3) )


#
def integrate3D_on_tetrahedra_formulas(polynom):
    '''
    '''
    val = 0

    monoms = get_all_monoms(polynom)

    for monom in monoms:
        val += integrate3D_on_tetrahedra_monomformula(monom)

    return val

def integrate_on_tetrahedra(polynom):
    '''
    '''
    USE_SYMBOLIC_INTEGRATION =  False

    if USE_SYMBOLIC_INTEGRATION :
        val = integrate3D_on_tetrahedra_symbolic(polynom)
    else:
        val = integrate3D_on_tetrahedra_formulas(polynom)

    return val

#
# C output of vectors/matrices...
#

def to_c_vector(vector):
    '''
    '''
    dim1 = vector.shape[0]
    dim2 = vector.shape[1]

    # get the right format first...
    fmt =  "%10s.0"
    leng = -1
    for i in range(dim1):
        if len(str(vector[i,0])) > 10:
            fmt = "%" + "%d" % (len(str(vector[i,0]))+1) + "s" + ".0"
    # get the right format first...

    xstr = "   {"
    for i in range(dim1):
        xstr += fmt % str(vector[i,0])
        if i != dim1-1 :
            xstr +=  " , "
    xstr += " };"

    return xstr

def to_c_matrix(matrix):
    '''
    '''
    dim1 = matrix.shape[0]
    dim2 = matrix.shape[1]

    # get the right format first...
    fmt =  "%10s.0"
    leng = -1
    for i in range(dim1):
        for j in range(dim2):
            if len(str(matrix[i,j])) > 10:
                fmt = "%" + "%d" % (len(str(matrix[i,j]))+1) + "s" + ".0"
    # get the right format first...

    xstr = ""

    for i in range(dim1):
        xstr += "   {"
        for j in range(dim2):
            xstr +=  fmt % str(matrix[i,j])
            if j != dim2-1 :
                xstr +=  " , "
        xstr +=  " } , \n"

    return xstr

def integrate2D(polynom):
    ip_ = integrate(polynom, x)
    #print "int on x = ", ip_
    ip_0_1my = ip_.subs(x, 1-y) - ip_.subs(x, 0)
    #print "int on x from 0 to 1-y = ", ip_0_1my
    ip_0_1my_ = integrate(ip_0_1my, y)
    #print "int on y = ", ip_0_1my_
    ip_0_1my_0_1 = ip_0_1my_.subs(y, 1) - ip_0_1my_.subs(y, 0)
    #print "int on y from 0 to 1 = ", ip_0_1my_0_1
    return ip_0_1my_0_1

def integrate3D(polynom):
    ip_ = integrate(polynom, x)
    #print "int on x = ", ip_
    ip_0_1mymz = ip_.subs(x, 1-y-z) - ip_.subs(x, 0)
    #print "int on x from 0 to 1-y-z = ", ip_0_1mymz
    ip_0_1mymz_ = integrate(ip_0_1mymz, y)
    #print "int on y = ", ip_0_1mymz_
    ip_0_1mymz_0_1mz = ip_0_1mymz_.subs(y, 1-z) - ip_0_1mymz_.subs(y, 0)
    #print "int on y from 0 to 1-z = ", ip_0_1mymz_0_1mz
    ip_0_1mymz_0_1mz_ = integrate(ip_0_1mymz_0_1mz, z)
    #print "int on z = ", ip_0_1mymz_0_1mz_
    ip_0_1mymz_0_1mz_0_1 = ip_0_1mymz_0_1mz_.subs(z, 1) - ip_0_1mymz_0_1mz_.subs(z, 0)
    #print "int on z from 0 to 1 = ", ip_0_1mymz_0_1mz_0_1
    return ip_0_1mymz_0_1mz_0_1


def test_integrate3D_method1(n, p):
    t1 = time.time()
    for k in range(n): integrate3D(p)
    t2 = time.time()
    return t2-t1

def test_integrate3D_method2(n, p):
    t1 = time.time()
    for k in range(n): integrate_on_tetrahedra(p)
    t2 = time.time()
    return t2-t1

if __name__ == '__main__':
    '''
    test speed or symbolic integration / numeric integration(monom formula)
    '''
    p = 1 + 3*x + 4*y + 5*z + 6*x*y + 7*y*z + 8*x*z + 9*x*x + 10*y*y + 11*z*z + 12*x*x*x + 13*y*y*y + 14*z*z*z + 15*x*y*z + 16*x*x*y + 17*x*y*y + 18*x*x*z + 19*x*z*z + 20*y*y*z + 21*y*z*z + 22*x*x*x*x + 23*y*y*y*y + 24*z*z*z*z
    print("symbolic integration : time = ", test_integrate3D_method1( 20, p), 10*" ", "numeric integration  : time = ", test_integrate3D_method2(20, p))

    import sympy
    pp = sympy.polys.Poly(p,x,y,z)
    for k, m in enumerate(zip(pp.iter_monoms(), pp.iter_coeffs())):
        print(k, ' -> ', m)
    #for k, m in enumerate(pp.iter_monoms()):
    #    print(k, ' -> ', m, m.coeff())
    for k, m in enumerate(p.as_coeff_factors()[1]): #for monom in polynom:
        print(k, ' -> ', m)
        mm =  sympy.polys.Poly(m,x,y,z)
        for c in mm.iter_coeffs(): print(c)



    p = 1 + 3*x + 4*y + 5*z + 6*x*y + 7*y*z + 8*x*z + 9*x*x + 10*y*y + 11*z*z + 12*x*x*x + 13*y*y*y + 14*z*z*z + 15*x*y*z
    print("symbolic integration : time = ", test_integrate3D_method1( 50, p), 10*" ", "numeric integration  : time = ", test_integrate3D_method2(50, p))

    p = 1 + 3*x + 4*y + 5*z
    print("symbolic integration : time = ", test_integrate3D_method1(200, p), 10*" ", "numeric integration  : time = ", test_integrate3D_method2(200, p))

    p = 1 + 3*x 
    print("symbolic integration : time = ", test_integrate3D_method1(200, p), 10*" ", "numeric integration  : time = ", test_integrate3D_method2(200, p))



