#
#
from sys import *
from math import *
#
from libmeschach      import *
from libmeschach_adds import *
from libmeschach_ef   import *
#
#
import time
import profile
#
#-------------------------------------------------------------------
#
def toto(x):
	return 10/4.0*(1.0/2.0-(x-0.5)*(x-0.5) );
#
N = 200000
x = 0.2
#
#
MyFun1 = Fun1D_get()
Fun1D_setFunctionPython(MyFun1, toto)
#
MyFun2 = Fun1D_get()
Fun1D_setCFunction(MyFun2, Phi1D_08)
#
MyFun3 = Fun1D_get()
Fun1D_setFunctionPython(MyFun3, lambda x:10/4.0*(1.0/2.0-(x-0.5)*(x-0.5) ))
#
def Fun1D_test( fun , x ):
    #
    a = time.time()
    #
    for i in range(N):
        Fun1D_evalFunction(fun, x)
    #
    b = time.time()
    #
    print "time test = ", b-a
#
#
print "begin"
#
Fun1D_test( MyFun1, x)
Fun1D_test( MyFun2, x)
Fun1D_test( MyFun3, x)
#
print "end"
#
profile.run('Fun1D_test( MyFun1, x)')
profile.run('Fun1D_test( MyFun2, x)')
profile.run('Fun1D_test( MyFun3, x)')
#
#