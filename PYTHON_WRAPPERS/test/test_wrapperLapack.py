
import sys

from meschach        import *
from meschach_lapack import *
from meschach_adds   import *



A = sp_get(5,5,3)
A = sp_tridiag(A, 1,4,3)

M = m_get(5,5)

sp_m2dense(A,M)

m_foutput(sys.stdout, M)


B = bd_get(5,1,1)
B = mat2band(M,1,1,B)
b_foutput(sys.stdout, B)

C = sp2band(A)
b_foutput(sys.stdout, C)

D = sp2lowerband(A)
b_foutput(sys.stdout, D)
