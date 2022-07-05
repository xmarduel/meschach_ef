
from sys import *
from time import *

from meschach import *
from meschach_adds import *
from meschach_ef import *
from meschach_spooles import *


# pair
NX = 2*16*16*1


method = "MESCHACH"
#method = "SPOOLES"
#method = "AMD"

# ------------------------------------
def fill_sparse_matrix(A):

    NX = SPMAT_m_get(A)

    for i in range(NX):
        sp_set_val(A, i,i, 4.0)

    for i in range(NX-1):
        sp_set_val(A, i,i+1, -1.0)
        sp_set_val(A, i+1,i, -1.0)

    # plus junk value
    sp_set_val(A, 0,NX-1, -2.0)
    sp_set_val(A, NX-1,0, -2.0)

    sp_set_val(A,1,NX-2, -2.0)
    sp_set_val(A,NX-2,1, -2.0)

    #plus
    for i in range(NX/2):
        sp_set_val(A, i,i+NX/2, -1.0)
        sp_set_val(A, i+NX/2,i, -1.0)
# ------------------------------------

P    = px_get(NX)
INVP = px_get(NX)

A    = sp_get(NX,NX,4)
PAPt = sp_get(NX,NX,4)
TMP  = sp_get(NX,NX,4)


x = v_get(NX)
y = v_get(NX)

x1 = v_get(NX)
x2 = v_get(NX)

diff = v_get(NX)


fill_sparse_matrix(A)


v_ones(x)

sp_mv_mlt(A,x,y) # y = A.x


# SOLVE LINEAR SYSTEM - NO BANDWR -
time1 = time()

print("without bandwr ... ")
LLT_1 = sp_copy(A)
spCHfactor(LLT_1)
print("without bandwr, nnz(LLT) = ", sp_nonzeros(LLT_1)
print("without bandwr, nnz(LLT) (sym) = ", sp_nonzeros_symmetric_part(LLT_1), "\n")
time2 = time()
print("without bandwr, time CHOLESKY FACTORISATION = ", time2 - time1)

x1 = spCHsolve(LLT_1,y,x1)

# CHECK SOL :
res = v_norm2( v_sub(x1, x, diff) )
print("the diff of x-x1 = ", res)


time1 = time()
#-----------------------------------
if ( method == "MESCHACH"):
   sp_bandwr_meschach(A, P, INVP, BANDWRe_MESCHACH_PROFILE) # BANDWRe_MESCHACH_BAND
elif ( method == "SPOOLES" ):
   sp_bandwr_spooles(A, P, INVP, 1)
elif ( method == "AMD"):
   sp_bandwr_amd(A, P, INVP, 0)
else:
   exit(0)
#-----------------------------------
time2 = time()
print("\ntime for permutation evaluation (band reduction) = ", time2-time1)


# Apply the permutation on the SPARSE MTX
px_sp_rows(P, A, TMP)
px_sp_cols(INVP, TMP, PAPt )
#sp_foutput(stdout, PAPt)

graphics_matrix_pattern ("X11", A   , "z1")
graphics_matrix_pattern ("X11", PAPt, "z2")
#-----------------

# SOLVE LINEAR SYSTEM - WITH BANDWR -

time1 = time()

LLT_2 = sp_copy(PAPt)
spCHfactor(LLT_2)
print("with bandwr, nnz(LLT_2) = ", sp_nonzeros(LLT_2))
print("with bandwr, nnz(LLT_2) (sym) = ", sp_nonzeros_symmetric_part(LLT_2), "\n")
time2 = time()
print("with bandwr, time CHOLESKY FACTORISATION = ", time2 - time1)

Py = v_get(NX)
Px = v_get(NX)

Py = px_vec(P, y, Py)
spCHsolve(LLT_2, Py, Px)
x2 = px_vec(INVP,Px,x2)


# CHECK SOL :
res = v_norm2( v_sub(x2, x, diff) )
print("the diff of x-x2 = ", res)
