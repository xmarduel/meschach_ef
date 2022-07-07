
import sys
import time

from meschach import *
from meschach_adds import *
from meschach_ef import *
from meschach_spooles import *


# pair
NX = 8*16*16
NX = 8
# ------------------------------------
def fill_matrix(A):
    NX = MAT_m_get(A)

    for i in range(NX):
        m_set_val(A, i,i, 4.0)

    for i in range(NX-1):
        m_set_val(A, i,i+1, -1.0)
        m_set_val(A, i+1,i, -1.0)

    # plus junk value
    m_set_val(A, 0,NX-1, -2.0)
    m_set_val(A, NX-1,0, -2.0)

    m_set_val(A,1,NX-2, -2.0)
    m_set_val(A,NX-2,1, -2.0)

    #plus
    for i in range(NX//2):
        m_set_val(A, i,i+NX//2, -1.0)
        m_set_val(A, i+NX//2,i, -1.0)
# ------------------------------------

A    = m_get(NX,NX)
PAPt = m_get(NX,NX)
TMP1 = m_get(NX,NX)

P    = px_get(NX)
INVP = px_get(NX)

B     = sp_get(NX,NX,4)
PBPt  = sp_get(NX,NX,4)
TMP2  = sp_get(NX,NX,4)


fill_matrix(A)
dense2sp(A,B)


method = "MESCHACH"


time1 = time.time()
#-----------------------------------
if ( method == "MESCHACH"):
   sp_bandwr_meschach(B, P, INVP, BANDWRe_MESCHACH_PROFILE) # BANDWRe_MESCHACH_BAND
elif ( method == "SPOOLES" ):
   sp_bandwr_spooles(B, P, INVP, 1)
elif ( method == "AMD"):
   sp_bandwr_amd(B, P, INVP, 0)
else:
   exit(0)
#-----------------------------------
time2 = time.time()
print("\ntime for permutation evaluation (band reduction) = ", time2-time1)

#px_foutput(sys.stdout,P)
#px_foutput(sys.stdout,INVP)

# evaluate the new matrix PAPt = P.A.INVP - Check the permutation on the DENSE MTX
px_rows(P, A, TMP1)
px_cols(INVP, TMP1, PAPt)
m_foutput(sys.stdout, PAPt)

# Check the permutation on the SPARSE MTX
px_sp_rows(P, B, TMP2)
px_sp_cols(INVP, TMP2, PBPt )
sp_foutput(sys.stdout, PBPt)
