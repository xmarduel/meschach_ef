#
from libmeschach        import *
from libmeschach_lapack import *
from libmeschach_adds   import *
#
from sys import *
#
#
#
A = sp_get(5,5,3);
A = sp_tridiag(A, 1,4,3)
#
M = m_get(5,5)
#
sp_m2dense(A,M)
#
m_foutput(stdout,M)
#
#
#
B = bd_get(5,1,1)
B = mat2band(M,1,1,B)
b_foutput(stdout, B)
#
#
C = sp2band(A)
b_foutput(stdout, C)
#
D = sp2lowerband(A)
b_foutput(stdout, D)
#