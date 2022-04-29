
from sympy import *

from ef3D_Pn import *
import ef3D_P1



class ef3D_P2(ef3D_Pn):
    '''
    '''
    DoF      = 10 
    ef_type  = "P2"

    a0 = Symbol('a0')
    a1 = Symbol('a1')
    a2 = Symbol('a2')
    a3 = Symbol('a3')
    a4 = Symbol('a4')
    a5 = Symbol('a5')
    a6 = Symbol('a6')
    a7 = Symbol('a7')
    a8 = Symbol('a8')
    a9 = Symbol('a9')

    pts = [ [0,0,0] , \
            [1,0,0] , \
            [0,1,0] , \
            [0,0,1] , \
            [Rational(1,2),0,0] , \
            [0,Rational(1,2),0] , \
            [0,0,Rational(1,2)] , \
            [Rational(1,2),Rational(1,2),0] , \
            [0,Rational(1,2),Rational(1,2)] , \
            [Rational(1,2),0,Rational(1,2)] ]


    def fun(cls, X, Y, Z):
        '''
        fun = u1*N1 + u2*N2 + u3*N3 + u4*N4 + u5*N5 + u6*N7 + u7*N7 + u8*N8 + u9*N9 + u10*N10
        '''
        return cls.a0 + cls.a1*X + cls.a2*Y + cls.a3*Z + cls.a4*X*X + cls.a5*Y*Y + cls.a6*Z*Z + cls.a7*X*Y + cls.a8*Y*Z + cls.a9*X*Z

    fun = classmethod(fun)


    def __init__(self):
        '''
        '''
        self.FunBase = self.getFunBase()
        self.efM1    = ef3D_P1.ef3D_P1()


    def getFunBase(self):
        '''
        '''
        a = Matrix([[self.a0], [self.a1], [self.a2], [self.a3], [self.a4], [self.a5], [self.a6], [self.a7], [self.a8], [self.a9]])
    
        c = Matrix([[1], [x], [y], [z], [x**2], [y**2], [z**2], [x*y], [y*z], [x*z]])
    
    
        v = Matrix( [ self.fun(*pt) for pt in self.pts ] )
    
        A = zeros(self.DoF)
        for i in range(self.DoF):
            for j in range(self.DoF):
                A[i,j] = diff(v[i], a[j])
    
        Ainv  = A.inv()
        AinvT = Ainv.T
    
        FunBase = AinvT * c
    
        return FunBase

