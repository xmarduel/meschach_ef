
from sympy import *

from ef3D_Pn import *
import ef3D_P1
import ef3D_P2



class ef3D_P3(ef3D_Pn):
    '''
    '''
    DoF      = 20
    ef_type  = "P3"

    a00 = Symbol('a00')
    a01 = Symbol('a01')
    a02 = Symbol('a02')
    a03 = Symbol('a03')
    a04 = Symbol('a04')
    a05 = Symbol('a05')
    a06 = Symbol('a06')
    a07 = Symbol('a07')
    a08 = Symbol('a08')
    a09 = Symbol('a09')
    a10 = Symbol('a10')
    a11 = Symbol('a11')
    a12 = Symbol('a12')
    a13 = Symbol('a13')
    a14 = Symbol('a14')
    a15 = Symbol('a15')
    a16 = Symbol('a16')
    a17 = Symbol('a17')
    a18 = Symbol('a18')
    a19 = Symbol('a19')

    pts = [ [0,0,0] , \
            [1,0,0] , \
            [0,1,0] , \
            [0,0,1] , \
            [Rational(1,3),0,0] , \
            [0,Rational(1,3),0] , \
            [0,0,Rational(1,3)] , \
            [Rational(2,3),0,0] , \
            [0,Rational(2,3),0] , \
            [0,0,Rational(2,3)] , \
            [0,Rational(1,3),Rational(1,3)] , \
            [Rational(1,3),0,Rational(1,3)] , \
            [Rational(1,3),Rational(1,3),0] , \
            [0,Rational(1,3),Rational(2,3)] , \
            [0,Rational(2,3),Rational(1,3)] , \
            [Rational(1,3),0,Rational(2,3)] , \
            [Rational(2,3),0,Rational(1,3)] , \
            [Rational(1,3),Rational(2,3),0] , \
            [Rational(2,3),Rational(1,3),0] , \
            [Rational(1,3),Rational(1,3),Rational(1,3)] ]


    def fun(cls, X, Y, Z):
        '''
        fun = u1*N1 + u2*N2 + u3*N3 + u4*N4 + u5*N5 + u6*N7 + u7*N7 + u8*N8 + u9*N9 + u10*N10 + u11*N11 + u12*N12 + u13*N13 + u14*N14 + u15*N15 + u16*N16 + u17*N17 + u18*N18 + u19*N19 + u20*N20
        '''
        return cls.a00 + cls.a01*X + cls.a02*Y + cls.a03*Z + \
               cls.a04*X*X + cls.a05*Y*Y + cls.a06*Z*Z + cls.a07*X*Y + cls.a08*Y*Z + cls.a09*X*Z + \
               cls.a10*X*X*X + cls.a11*Y*Y*Y + cls.a12*Z*Z*Z + \
               cls.a13*X*X*Y + cls.a14*X*Y*Y + cls.a15*Y*Y*Z + cls.a16*Y*Z*Z + cls.a17*X*X*Z + cls.a18*X*Z*Z + cls.a19*X*Y*Z

    fun = classmethod(fun)


    def __init__(self):
        '''
        '''
        self.FunBase = self.getFunBase()
        self.efM1    = ef3D_P2.ef3D_P2()


    def getFunBase(self):
        '''
        '''
        a = Matrix([[self.a00], [self.a01], [self.a02], [self.a03], [self.a04], [self.a05], [self.a06], [self.a07], [self.a08], [self.a09], [self.a10], [self.a11], [self.a12], [self.a13], [self.a14], [self.a15], [self.a16], [self.a17], [self.a18], [self.a19]] )
    
        c = Matrix([[1],        [x],        [y],        [z],        [x*x],      [y*y],      [z*z],      [x*y],      [y*z],      [x*z],      [x*x*x],    [y*y*y],    [z*z*z],    [x*x*y],    [x*y*y],    [y*y*z],    [y*z*z],    [x*x*z],    [x*z*z],    [x*y*z]])
    
        v = Matrix( [ self.fun(*pt) for pt in self.pts ] )
    
    
        A = zeros(self.DoF)
        for i in range(self.DoF):
            for j in range(self.DoF):
                A[i,j] = diff(v[i], a[j])
    
        Ainv  = A.inv()
        AinvT = Ainv.T
    
    
        FunBase = AinvT * c
    
        return FunBase
#

