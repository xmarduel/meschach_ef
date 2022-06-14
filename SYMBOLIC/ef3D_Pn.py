
from sympy import *

from ef3D_utils import *


x = symbols('x')
y = symbols('y')
z = symbols('z')


class ef3D_Pn(object):
    '''
    '''
    def __init__(self):
        '''
        '''
        self.DoF     = 0   # to be set in derived class ef3D_P[1-2-3] :   4 or 10 or 20
        self.ef_type = ""  # to be set in derived class ef3D_P[1-2-3] : "P1"  "P2"  "P3"
    
        self.FunBase = None # to be evaluated in derived class ef3D_P[1-2-3]
    
        self.efM1    = None # to be evaluated in derived class ef3D_P[1-2-3]
    
        self.matr_fmt_item = "%10s.0" # to be set in derived class ef3D_P[1-2-3]


    def getMatrixMass(self):
        '''
        '''
        Mass  = self.FunBase * self.FunBase.T
    
        return Mass

    def getMatrixStiff(self):
        '''
        '''
        dFunBasedx = self.FunBase.applyfunc(lambda v: diff(v, x) )
        dFunBasedy = self.FunBase.applyfunc(lambda v: diff(v, y) )
        dFunBasedz = self.FunBase.applyfunc(lambda v: diff(v, z) )
    
        Stiff_xx = dFunBasedx * dFunBasedx.T
        Stiff_yy = dFunBasedy * dFunBasedy.T
        Stiff_zz = dFunBasedz * dFunBasedz.T
        Stiff_xy = dFunBasedx * dFunBasedy.T
        Stiff_yx = dFunBasedy * dFunBasedx.T
        Stiff_yz = dFunBasedy * dFunBasedz.T
        Stiff_zy = dFunBasedz * dFunBasedy.T
        Stiff_xz = dFunBasedx * dFunBasedz.T
        Stiff_zx = dFunBasedz * dFunBasedx.T
    
        return [ Stiff_xx, Stiff_yy, Stiff_zz, Stiff_xy, Stiff_yx, Stiff_yz, Stiff_zy, Stiff_xz, Stiff_zx ]

    def getMatrixConv(self):
        '''
        '''
        dFunBasedx = self.FunBase.applyfunc(lambda v: diff(v, x) )
        dFunBasedy = self.FunBase.applyfunc(lambda v: diff(v, y) )
        dFunBasedz = self.FunBase.applyfunc(lambda v: diff(v, z) )
    
        Conv_x = self.FunBase * dFunBasedx.T
        Conv_y = self.FunBase * dFunBasedy.T
        Conv_z = self.FunBase * dFunBasedz.T
    
        return [ Conv_x, Conv_y, Conv_z ]

    def getMatrixConvT(self):
        '''
        '''
        dFunBasedx = self.FunBase.applyfunc(lambda v: diff(v, x) )
        dFunBasedy = self.FunBase.applyfunc(lambda v: diff(v, y) )
        dFunBasedz = self.FunBase.applyfunc(lambda v: diff(v, z) )
    
        ConvT_x = dFunBasedx * self.FunBase.T
        ConvT_y = dFunBasedy * self.FunBase.T
        ConvT_z = dFunBasedz * self.FunBase.T
    
        return [ ConvT_x, ConvT_y, ConvT_z ]

    def getMatrixConvM1(self):
        '''
        '''
        dFunBasedx = self.FunBase.applyfunc(lambda v: diff(v, x) )
        dFunBasedy = self.FunBase.applyfunc(lambda v: diff(v, y) )
        dFunBasedz = self.FunBase.applyfunc(lambda v: diff(v, z) )
    
        Conv_x = self.efM1.FunBase * dFunBasedx.T
        Conv_y = self.efM1.FunBase * dFunBasedy.T
        Conv_z = self.efM1.FunBase * dFunBasedz.T
    
        return [ Conv_x, Conv_y, Conv_z ]

    def getTensorMass(self):
        '''
        '''
        Mass = self.getMatrixMass()
    
        Tensor = [ ]
    
        for k in range(self.DoF):
            Tensor.append( Mass.applyfunc( lambda v: (v*self.FunBase[k]).expand()) )
    
        return Tensor

    def getTensorConvT(self):
        '''
        '''
        Conv = self.getMatrixConvT()
    
        ConvT_x = Conv[0]
        ConvT_y = Conv[1]
        ConvT_z = Conv[2]
    
        Tensor_x = [ ]
        Tensor_y = [ ]
        Tensor_z = [ ]
    
        for k in range(self.DoF):
            Tensor_x.append( ConvT_x.applyfunc( lambda v: (v*self.FunBase[k]).expand() ) )
            Tensor_y.append( ConvT_y.applyfunc( lambda v: (v*self.FunBase[k]).expand() ) )
            Tensor_z.append( ConvT_z.applyfunc( lambda v: (v*self.FunBase[k]).expand() ) )
    
        return [ Tensor_x, Tensor_y, Tensor_z ]



    def bf_cdef(self):
        '''
        '''
        dFunBasedx = self.FunBase.applyfunc(lambda v: diff(v, x) )
        dFunBasedy = self.FunBase.applyfunc(lambda v: diff(v, y) )
        dFunBasedz = self.FunBase.applyfunc(lambda v: diff(v, z) )
    
        print("/*--------------------------------------------------------*/")
        print("/*--------------------------------------------------------*/")
        for k, fb in enumerate(self.FunBase):
            print("")
            print("Real F%d_3D_%s(Real x, Real y, Real z)" % (k, self.ef_type))
            print("{")
            print("    return %s;" % printing.ccode(fb))
            print("}")
        print("")
    
        print("/*--------------------------------------------------------*/")
        print("/*--------------------------------------------------------*/")
        for k, fb in enumerate(dFunBasedx):
            print("")
            print("Real DFDX%d_3D_%s(Real x, Real y, Real z)" % (k, self.ef_type))
            print("{")
            print("    return %s;" % printing.ccode(fb))
            print("}")
        print("")
    
        print("/*--------------------------------------------------------*/")
        print("/*--------------------------------------------------------*/")
        for k, fb in enumerate(dFunBasedy):
            print("")
            print("Real DFDY%d_3D_%s(Real x, Real y, Real z)" % (k, self.ef_type))
            print("{")
            print("    return %s;" % printing.ccode(fb))
            print("}")
        print("")
    
        print("/*--------------------------------------------------------*/")
        print("/*--------------------------------------------------------*/")
        for k, fb in enumerate(dFunBasedz):
            print("")
            print("Real DFDZ%d_3D_%s(Real x, Real y, Real z)" % (k, self.ef_type))
            print("{")
            print("    return %s;" % printing.ccode(fb))
            print("}")
        print("")
        print("/*--------------------------------------------------------*/")
        print("/*--------------------------------------------------------*/")


    def mx_cdef_vect(self):
        '''
        '''
        IVec = self.FunBase.applyfunc( integrate_on_tetrahedra )
    
        print("Real VEC_Masse_%s[%d] = " % (self.ef_type, self.DoF))
        print("")
        print(to_c_vector(IVec))
        print("")

    def mx_cdef_mass(self):
        '''
        '''
        Mass   = self.getMatrixMass()
    
        IMass = Mass.applyfunc( integrate_on_tetrahedra )
    
        print("Real MAT_Masse_%s[%d][%d] = " % (self.ef_type, self.DoF, self.DoF))
        print("{\n" + to_c_matrix(IMass) + "};")
        print("")
        print("")

    def mx_cdef_stiff(self):
        '''
        '''
        Stiff  = self.getMatrixStiff()
    
        Stiff_xx = Stiff[0]
        Stiff_yy = Stiff[1]
        Stiff_zz = Stiff[2]
        Stiff_xy = Stiff[3]
        Stiff_yx = Stiff[4]
        Stiff_yz = Stiff[5]
        Stiff_zy = Stiff[6]
        Stiff_xz = Stiff[7]
        Stiff_zx = Stiff[8]
    
        IStiff_xx = Stiff_xx.applyfunc( integrate_on_tetrahedra )
        IStiff_yy = Stiff_yy.applyfunc( integrate_on_tetrahedra )
        IStiff_zz = Stiff_zz.applyfunc( integrate_on_tetrahedra )
        IStiff_xy = Stiff_xy.applyfunc( integrate_on_tetrahedra )
        IStiff_yx = Stiff_yx.applyfunc( integrate_on_tetrahedra )
        IStiff_xz = Stiff_xz.applyfunc( integrate_on_tetrahedra )
        IStiff_zx = Stiff_zx.applyfunc( integrate_on_tetrahedra )
        IStiff_yz = Stiff_yz.applyfunc( integrate_on_tetrahedra )
        IStiff_zy = Stiff_zy.applyfunc( integrate_on_tetrahedra )
    
        print("Real MAT_Rigid_%s[9][%d][%d] = " % (self.ef_type, self.DoF, self.DoF))
        print("{")
        print("   { /* Stiff_xx */ \n" + to_c_matrix(IStiff_xx) + "   },")
        print("   { /* Stiff_xy */ \n" + to_c_matrix(IStiff_xy) + "   },")
        print("   { /* Stiff_xz */ \n" + to_c_matrix(IStiff_xz) + "   },")
        print("   { /* Stiff_yx */ \n" + to_c_matrix(IStiff_yx) + "   },")
        print("   { /* Stiff_yy */ \n" + to_c_matrix(IStiff_yy) + "   },")
        print("   { /* Stiff_yz */ \n" + to_c_matrix(IStiff_yz) + "   },")
        print("   { /* Stiff_zx */ \n" + to_c_matrix(IStiff_zx) + "   },")
        print("   { /* Stiff_zy */ \n" + to_c_matrix(IStiff_zy) + "   },")
        print("   { /* Stiff_zz */ \n" + to_c_matrix(IStiff_zz) + "   },")
        print("};")
        print("")

    def mx_cdef_conv(self):
        '''
        '''
        Conv   = self.getMatrixConv()
    
        Conv_x   = Conv[0]
        Conv_y   = Conv[1]
        Conv_z   = Conv[2]
    
        IConv_x = Conv_x.applyfunc( integrate_on_tetrahedra )
        IConv_y = Conv_y.applyfunc( integrate_on_tetrahedra )
        IConv_z = Conv_z.applyfunc( integrate_on_tetrahedra )
    
        print("Real MAT_Conv_x_%s[%d][%d] = " % (self.ef_type, self.DoF, self.DoF))
        print("{")
        print( to_c_matrix(IConv_x))
        print("};")
        print("")
        print("Real MAT_Conv_y_%s[%d][%d] = " % (self.ef_type, self.DoF, self.DoF))
        print("{")
        print( to_c_matrix(IConv_y))
        print("};")
        print("")
        print("Real MAT_Conv_z_%s[%d][%d] = " % (self.ef_type, self.DoF, self.DoF))
        print("{")
        print(to_c_matrix(IConv_z))
        print("};")
        print("")

    def mx_cdef_convM1(self):
        '''
        '''
        ConvM1 = self.getMatrixConvM1()
    
        ConvM1_x   = ConvM1[0]
        ConvM1_y   = ConvM1[1]
        ConvM1_z   = ConvM1[2]
    
        IConvM1_x = ConvM1_x.applyfunc( integrate_on_tetrahedra )
        IConvM1_y = ConvM1_y.applyfunc( integrate_on_tetrahedra )
        IConvM1_z = ConvM1_z.applyfunc( integrate_on_tetrahedra )
    
        print("Real MAT_Conv_x_%sd%s[%d][%d] = " % (self.efM1.ef_type, self.ef_type, self.efM1.DoF, self.DoF))
        print("{")
        print(to_c_matrix(IConvM1_x))
        print("};")
        print("")
        print("Real MAT_Conv_y_%sd%s[%d][%d] = " % (self.efM1.ef_type, self.ef_type, self.efM1.DoF, self.DoF))
        print("{")
        print( to_c_matrix(IConvM1_y))
        print("};")
        print("")
        print("Real MAT_Conv_z_%sd%s[%d][%d] = " % (self.efM1.ef_type, self.ef_type, self.efM1.DoF, self.DoF))
        print("{")
        print( to_c_matrix(IConvM1_z))
        print("};")
        print("")

    def mx_cdef_tensor_mass(self):
        '''
        '''
        TensorMass  = self.getTensorMass()
    
        ITensorMass = [ matr.applyfunc( integrate_on_tetrahedra ) for matr in TensorMass]
    
        print("Real TENSOR_Mass_%s[%d][%d][%d] = " % (self.ef_type, self.DoF, self.DoF, self.DoF))
        print("{")
        for k in range(self.DoF):
            print("   { /* N%d x Nj x Nk */ \n" % k + to_c_matrix(ITensorMass[k]) + "   },")
        print("};")
        print("")


    def mx_cdef_tensor_convT(self):
        '''
        '''
        TensorConvT = self.getTensorConvT()
    
        TensorConvT_x = TensorConvT[0]
        TensorConvT_y = TensorConvT[1]
        TensorConvT_z = TensorConvT[2]
    
        ITensorConvT_x = [ matr.applyfunc( integrate_on_tetrahedra ) for matr in TensorConvT_x]
        ITensorConvT_y = [ matr.applyfunc( integrate_on_tetrahedra ) for matr in TensorConvT_y]
        ITensorConvT_z = [ matr.applyfunc( integrate_on_tetrahedra ) for matr in TensorConvT_z]
    
        print("Real TENSOR_ConvX_%s[%d][%d][%d] = " % (self.ef_type, self.DoF, self.DoF, self.DoF))
        print("{")
        for k in range(self.DoF):
            print("   { /* N%d x dNjdx x Nk */ \n" % k + to_c_matrix(ITensorConvT_x[k]) + "   },")
        print("};")
        print("")
        print("Real TENSOR_ConvY_%s[%d][%d][%d] = " % (self.ef_type, self.DoF, self.DoF, self.DoF))
        print("{")
        for k in range(self.DoF):
            print("   { /* N%d x dNjdy x Nk */ \n" % k + to_c_matrix(ITensorConvT_y[k]) + "   },")
        print("};")
        print("")
        print("Real TENSOR_ConvZ_%s[%d][%d][%d] = " % (self.ef_type, self.DoF, self.DoF, self.DoF))
        print("{")
        for k in range(self.DoF):
            print("   { /* N%d x dNjdz x Nk */ \n" % k + to_c_matrix(ITensorConvT_z[k]) + "   },")
        print("};")
        print("")


    def mx_cdef(self):
        '''
        '''
    
        print("")
        print("   /* -- Element %s ------------------------------------- */" % self.ef_type)
        print("")
    
        self.mx_cdef_vect()
    
        self.mx_cdef_mass()
        self.mx_cdef_stiff()
        self.mx_cdef_conv()
        self.mx_cdef_convM1()
    
        self.mx_cdef_tensor_mass()
        self.mx_cdef_tensor_convT()
    

    def testFunBase(self):
        '''
        '''
        for k, fb in enumerate(self.FunBase):
            print("----------------------------------------------")
            for pt in self.pts:
                print("pt = %s -> fb_%d(pt) = %s" % (str(pt), k, str(fb.subs({x:pt[0], y:pt[1], z:pt[2]}))))



# ---------------------------------------------------------------------------------
#
