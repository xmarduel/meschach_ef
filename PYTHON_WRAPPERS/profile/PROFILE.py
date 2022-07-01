
from sys import *
from math import *

from meschach      import *
from meschach_adds import *
from meschach_ef   import *


import time
import profile

#-------------------------------------------------------------------

N = 1000000
x = 0.2


MyFun1 = Rhs1D_get()
Rhs1D_setCFunction(MyFun1, 0, 0, Sol1D_05)

MyFun2 = Rhs1D_get()
Rhs1D_setFunctionPython(MyFun2, 0, 0, lambda x:10/4.0*(1.0/2.0-(x-0.5)*(x-0.5) ))

MyFun3 = Rhs1D_get()
Rhs1D_setLUAFunction(MyFun3, 0, 0, "10/4.0 * (1.0/2.0 - (x-0.5)*(x-0.5))")

def Fun1D_test( fun , x ):
    a = time.time()

    for i in range(N):
        Rhs1D_evalFunction(fun, 0, 0, x)

    b = time.time()

    print("time test = ", b-a)


print("begin")

Fun1D_test(MyFun1, x)
Fun1D_test(MyFun2, x)
Fun1D_test(MyFun3, x)

print("end")

profile.run('Fun1D_test( MyFun1, x)')
profile.run('Fun1D_test( MyFun2, x)')
profile.run('Fun1D_test( MyFun3, x)')

