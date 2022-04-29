#
#
from libmeschach import *
from libmeschach_adds import *
from libmeschach_ef import *
from libmeschach_spooles import *
#
from sys import *
from time import *
#
#
# pair
NX = 3*8
#
# ------------------------------------
def fill_matrix(A):
#
    NX = MAT_m_get(A)
    #
    for i in range(NX):
        for j in range(NX):
            m_set_val(A, i,j, i+1)
    #
# ------------------------------------        
#
A1 = m_get(NX,NX)
A2 = m_get(NX,NX)
#
#
B1 = sp_get(NX,NX,4)
B2 = sp_get(NX,NX,4)
#
#
fill_matrix(A1)
fill_matrix(A2)
#
dense2sp(A1,B1)
dense2sp(A2,B2)
#
sp_foutput(stdout, B1)
graphics_matrix_pattern("X11", B1, "ss")
sp_zero_subsp(B1, 3,6, 3,4)
sp_compact(B1, 0.0)
graphics_matrix_pattern("X11", B1, "ss")
#