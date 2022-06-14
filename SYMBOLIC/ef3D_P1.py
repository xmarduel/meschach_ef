#
from sympy import *
from sympy import __version__ as sympy_version

from ef3D_Pn import *



class ef3D_P1(ef3D_Pn):
    '''
    '''
    print("ef3D_P1 utilizing sympy %s" % sympy_version)

    DoF = 4

    ef_type = "P1"

    a0 = Symbol('a0')
    a1 = Symbol('a1')
    a2 = Symbol('a2')
    a3 = Symbol('a3')

    pts = [ [0,0,0] , \
            [1,0,0] , \
            [0,1,0] , \
            [0,0,1] ]


    @classmethod
    def fun(cls, X,Y,Z):
        '''
        fun = u1*N1 + u2*N2 + u3*N3 + u4*N4
        '''
        return cls.a0 + cls.a1*X + cls.a2*Y + cls.a3*Z

    def __init__(self):
        '''
        '''
        self.FunBase = self.getFunBase()
        self.efM1    = self


    def getFunBase(self):
        '''
        '''
        a  = Matrix([[self.a0], [self.a1], [self.a2], [self.a3]])
    
        c  = Matrix([[1], [x], [y], [z]])
    
    
        v = Matrix( [ self.fun(*pt) for pt in self.pts ] )
    
        A = zeros(self.DoF)
        for i in range(self.DoF):
            for j in range(self.DoF):
                A[i,j] = diff(v[i], a[j]) 
    
        Ainv  = A.inv()
        AinvT = Ainv.T
    
        FunBase = AinvT * c
    
        return FunBase

