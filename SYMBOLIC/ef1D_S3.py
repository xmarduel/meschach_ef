
from sympy import *
from sympy import __version__ as sympy_version

init_printing()

from sympy.printing.ccode import C99CodePrinter
printer = C99CodePrinter()


class ef1D_S3:
    '''
    '''
    print("ef1D_S3 integration utilizing sympy %s" % sympy_version)


    ef_type = "S3"

    t = Symbol('t')


    B1 = (1-t)**3
    B2 =  1 + 3*(1-t) + 3*(1-t)**2 - 3*(1-t)**3 
    B3 =  1 + 3*(t-0) + 3*(t-0)**2 - 3*(t-0)**3
    B4 =  t**3


    def __init__(self):
        '''
        '''
        self.FunBase = Matrix([ self.B1, self.B2, self.B3, self.B4])
        self.DFunBase = diff(self.FunBase) 
        self.DDFunBase = diff(self.DFunBase) 

        self.Mass  = self.FunBase * self.FunBase.T 
        self.Conv  = self.FunBase * self.DFunBase.T    
        self.Stiff = self.DFunBase * self.DFunBase.T
        self.Stiff2 = self.DDFunBase * self.DDFunBase.T

        self.I_xx = self.FunBase * self.DDFunBase.T
        self.x_xx = self.DFunBase * self.DDFunBase.T

        self.ConvT  = self.DFunBase * self.FunBase.T  

        self.TENSOR_CONV_1 = self.B1 * self.ConvT
        self.TENSOR_CONV_2 = self.B2 * self.ConvT
        self.TENSOR_CONV_3 = self.B3 * self.ConvT
        self.TENSOR_CONV_4 = self.B4 * self.ConvT

        self.TENSOR_MASS_1 = self.Mass * self.B1
        self.TENSOR_MASS_2 = self.Mass * self.B2
        self.TENSOR_MASS_3 = self.Mass * self.B3
        self.TENSOR_MASS_4 = self.Mass * self.B4
    

    def to_c(self, data):
        '''
        '''
        data  = data.replace("[", "{")
        data  = data.replace("]", "}")
        data  = data.replace("/", ".0/")

        return data

    def integrate(self):
        '''
        '''
        res = integrate(self.Mass, (self.t,0,1))
        print("Mass----")
        print(self.to_c(printer.doprint(res)))

        res = integrate(self.Conv, (self.t,0,1))
        print("Conv----")
        print(self.to_c(printer.doprint(res)))

        res = integrate(self.Stiff, (self.t,0,1))
        print("Stiff----")
        print(self.to_c(printer.doprint(res)))

        res = integrate(self.Stiff2, (self.t,0,1))
        print("Stiff2----")
        print(self.to_c(printer.doprint(res)))

        res = integrate(self.I_xx, (self.t,0,1))
        print("I_xx----")
        print(self.to_c(printer.doprint(res)))

        res = integrate(self.x_xx, (self.t,0,1))
        print("x_xx----")
        print(self.to_c(printer.doprint(res)))


        res = integrate(self.FunBase, (self.t,0,1))
        print("VEC_I----")
        print(self.to_c(printer.doprint(res)))

        res = integrate(self.DFunBase, (self.t,0,1))
        print("VEC_x----")
        print(self.to_c(printer.doprint(res)))

        res = integrate(self.DDFunBase, (self.t,0,1))
        print("VEC_xx----")
        print(self.to_c(printer.doprint(res)))


        res = integrate(self.TENSOR_CONV_1, (self.t,0,1))
        print("TENSOR_CONV_1----")
        print(self.to_c(printer.doprint(res)))
        res = integrate(self.TENSOR_CONV_2, (self.t,0,1))
        print("TENSOR_CONV_2----")
        print(self.to_c(printer.doprint(res)))
        res = integrate(self.TENSOR_CONV_3, (self.t,0,1))
        print("TENSOR_CONV_3----")
        print(self.to_c(printer.doprint(res)))
        res = integrate(self.TENSOR_CONV_4, (self.t,0,1))
        print("TENSOR_CONV_4----")
        print(self.to_c(printer.doprint(res)))


        res = integrate(self.TENSOR_MASS_1, (self.t,0,1))
        print("TENSOR_MASS_1----")
        print(self.to_c(printer.doprint(res)))
        res = integrate(self.TENSOR_MASS_2, (self.t,0,1))
        print("TENSOR_MASS_2----")
        print(self.to_c(printer.doprint(res)))
        res = integrate(self.TENSOR_MASS_3, (self.t,0,1))
        print("TENSOR_MASS_3----")
        print(self.to_c(printer.doprint(res)))
        res = integrate(self.TENSOR_MASS_4, (self.t,0,1))
        print("TENSOR_MASS_4----")
        print(self.to_c(printer.doprint(res)))



if __name__ == '__main__':
    ef = ef1D_S3()
    ef.integrate()






