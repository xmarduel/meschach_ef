
import sys

from meschach import *
from meschach_adds import *
from meschach_ef import *
from meschach_spooles import *


mem_info_on(TRUE)
mem_attach_list1()


NX = 3

M = m_get(NX,NX)

m_set_val(M, 0,0, 10.0)
m_set_val(M, 1,0, 11.0)
m_set_val(M, 2,0, 12.0)
m_set_val(M, 0,1, 20.0)
m_set_val(M, 1,1, 21.0)
m_set_val(M, 2,1, 22.0)
m_set_val(M, 0,2, 30.0)
m_set_val(M, 1,2, 31.0)
m_set_val(M, 2,2, 32.0)

print("Initial Matrix M :")
m_foutput(sys.stdout,M)


x = v_get(NX)

v_set_val(x, 0, 10)
v_set_val(x, 1, 11)
v_set_val(x, 2, 12)

print("Initial Vector x :")
v_foutput(sys.stdout, x);   # x = ( 10, 11, 12 )



P = px_get(NX)

px_transp(P,2,0)
px_transp(P,1,2)

MP  = m_get(3,3)
MIP = m_get(3,3)

m_zero(MP)
m_set_val(MP,0,2, 1.0)
m_set_val(MP,1,0, 1.0)
m_set_val(MP,2,1, 1.0)

m_zero(MIP)
m_set_val(MIP,0,1, 1.0)
m_set_val(MIP,1,2, 1.0)
m_set_val(MIP,2,0, 1.0)

print("Initial Permutation P :")
px_foutput(sys.stdout, P)   # P = [ 2 ; 0 ; 1 ]

print("Corresponding Matrix")
m_foutput(sys.stdout, MP)


IP = px_get(NX)

px_inv(P, IP)

print("The inverse of the Initial Permutation IP :")
px_foutput(sys.stdout, IP); # P = [ 1 ; 2 ; 0 ]

print("Corresponding Matrix")
m_foutput(sys.stdout, MIP)

print("check MP * MIP :")
ID = m_get(3,3)
m_zero(ID)
m_mlt(MIP,MP,ID)
m_foutput(sys.stdout, ID)

#--------------------------------------------------------

# Now tranformation of vector with the permutation
y = v_get(NX)

px_vec(P,x,y)

print("Vector resulting of the Permutation of V1 with P :")
v_foutput(sys.stdout, y)

print("Vector resulting of the Multiplication of M(P) with V1 :")
mv_mlt(MP,x,y)
v_foutput(sys.stdout, y)
#
# Now transformation of matrix the the permutation
#
M1 = m_get(NX,NX)
M2 = m_get(NX,NX)

print("test px_rows(P, M, M1) = M(P) * M ...")
px_rows(P, M, M1)
m_mlt(MP, M, M2)

print("Matrix resulting of the Rows-Permutation of M with P :")
m_foutput(sys.stdout, M1)
print("Matrix resulting of the Multiplication M(P) * M :")
m_foutput(sys.stdout, M2)



N1 = m_get(NX,NX)
N2 = m_get(NX,NX)

print("test px_cols(P, M, N) = M * M(P) ...")
px_cols(P, M, N1)
m_mlt(M, MP, N2)

print("Matrix resulting of the Cols-Permutation of M with P :")
m_foutput(sys.stdout, N1)
print("Matrix resulting of the Multiplication M * M(P) :")
m_foutput(sys.stdout, N2)

#################################################################

print("Now the same operation, but with Sparse Matrices ...")

S = sp_get(NX,NX,3)

sp_set_val(S, 0,0, 10.0)
sp_set_val(S, 1,0, 11.0)
sp_set_val(S, 2,0, 12.0)
sp_set_val(S, 0,1, 20.0)
sp_set_val(S, 1,1, 21.0)
sp_set_val(S, 2,1, 22.0)
sp_set_val(S, 0,2, 30.0)
sp_set_val(S, 1,2, 31.0)
sp_set_val(S, 2,2, 32.0)

print("Initial Matrix S :")
sp_foutput(sys.stdout,S)
#
# Now transformation of matrix the the permutation
#
S2 = sp_get(NX,NX,3)
S3 = sp_get(NX,NX,3)

R2 = sp_get(NX,NX,3)
R3 = sp_get(NX,NX,3)

SP = sp_get(NX,NX,1)

sp_set_val(SP, 0,2, 1.0)
sp_set_val(SP, 1,0, 1.0)
sp_set_val(SP, 2,1, 1.0)


px_sp_rows(P, S, S2)
sp_m_mlt(SP, S, R2)

print("Matrix resulting of the Rows-Permutation of S with P :")
sp_foutput(sys.stdout, S2)
print("Matrix resulting of the Multiplication S(P) * S :")
sp_foutput(sys.stdout, R2)

px_sp_cols(P, S, S3)  # px_cols permuts the rows of S with P !
sp_m_mlt(S, SP, R3)

print("Matrix resulting of the Cols-Permutation of S with P :")
sp_foutput(sys.stdout, S3)
print("Matrix resulting of the Multiplication S1 * S(P) :")
sp_foutput(sys.stdout, R3)

#
################################################################
#

print("Now check the bandWidth reduction ...")

NX = 8

K1 = sp_get(NX,NX,3)
K2 = sp_get(NX,NX,3)
K3 = sp_get(NX,NX,3)

sp_eye(K1);
sp_smlt(K1, 4.0, K1)

sp_set_val(K1, 0,3, 1.0)
sp_set_val(K1, 3,0, 1.0)
sp_set_val(K1, 1,6, 1.0)
sp_set_val(K1, 6,1, 1.0)

print("A sparse matrix ...")
sp_foutput(sys.stdout, K1)

Q  = px_get(NX)
IQ = px_get(NX)
#------------------------
sp_bandwr_meschach(K1, Q, IQ, 0)
#------------------------

print("The permutation is ")
px_foutput(sys.stdout, Q)
print("And its inverse is ")
px_foutput(sys.stdout, IQ)

#
# evaluate the new matrix K3 = P * K1 * IP
#
px_sp_cols(IQ, K1, K2)   #
px_sp_rows( Q, K2, K3)   #



#print("Permutated Matrix  K2 = P * K1 :"
#sp_foutput(sys.stdout, K2)
print("Permutated Matrix  K3 = P * K1 * P' :")
sp_foutput(sys.stdout, K3)


#------------------------
sp_bandwr_spooles(K1, Q, IQ, 0)
#------------------------

print("The permutation is (SPOOLES)")
px_foutput(sys.stdout, Q)
print("And its inverse is (SPOOLES)")
px_foutput(sys.stdout, IQ)

px_sp_cols(IQ, K1, K2)
px_sp_rows( Q, K2, K3)
#
#
#
#print("Permutated Matrix  K2 = P * K1 :"
#sp_foutput(sys.stdout, K2)
print("Permutated Matrix (SPOOLES) K3 = P * K1 * P' :")
sp_foutput(sys.stdout, K3)
#
#
#------------------------
sp_bandwr_amd(K1, Q, IQ, 0)
#------------------------
#
print("The permutation is (AMD)")
px_foutput(sys.stdout, Q)
print("And its inverse is (AMD)")
px_foutput(sys.stdout, IQ)

px_sp_cols(IQ, K1, K2)   #
px_sp_rows( Q, K2, K3)   #



#print("Permutated Matrix  K2 = P * K1 :")
#sp_foutput(sys.stdout, K2)
print("Permutated Matrix (AMD) K3 = P * K1 * P' :")
sp_foutput(sys.stdout, K3)