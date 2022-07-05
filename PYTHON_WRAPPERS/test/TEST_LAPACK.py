#! /usr/bin/env python

import sys
import math

from meschach         import *
from meschach_adds    import *
from meschach_ef      import *
from meschach_cpgplot import *
from meschach_libsciplot import *
from meschach_spooles import *
from meschach_lapack  import *

#-------------------------------------------------------------------

a = mem_attach_list1()
a = mem_attach_list2()
a = mem_attach_list3()
a = mem_attach_list4()
a = mem_attach_list5()
a = mem_attach_list6()
a = mem_attach_list7()

err_attach_list2()
err_attach_list3()
err_attach_list4()
err_attach_list5()
err_attach_list6()
err_attach_list7()

warn_attach_list2()
warn_attach_list3()
warn_attach_list4()
warn_attach_list5()
warn_attach_list6()
warn_attach_list7()

set_err_flag(EF_JUMP)
set_warn_flag(EF_SILENT)

#-------------------------------------------------------------------

N = 8

#-------------------------------------------------------------------

v1 = v_get(N)
v2 = v_get(N)

A = sp_get(N,N,3)

#--------------------------------------------------------------------

for i in range(0,N):
    sp_set_val(A,i,i, 5*(i+1) )

for i in range(0,N-1):
    sp_set_val(A,i,i+1, 3)  # 3 en haut
for i in range(0,N-1):
    sp_set_val(A,i+1,i, 3)  # 2 en bas

sp_foutput(sys.stdout,A)

B1 = sp2lowerband(A)
b_foutput(sys.stdout,B1)

B2 = sp2band(A)
b_foutput(sys.stdout,B2)


M1 = spmat2lapacklowerbanded(A)
M2 = spmat2lapackbanded     (A)

m_foutput(sys.stdout,M1)
m_foutput(sys.stdout,M2)



v1 = v_ones(v1)
v2 = sp_mv_mlt(A,v1,v2)

#v_foutput(sys.stdout,v1)
v_foutput(sys.stdout,v2)

v2 = v_ones(v2)
v2 = Ax_lapackbanded(A,v1,v2)
v_foutput(sys.stdout,v2)

v2 = v_zero(v2)
v2 = Ax_lapacklowerbanded(A,v1,v2)
v_foutput(sys.stdout,v2)

#
# free memory
# -----------

V_FREE(v1)
V_FREE(v2)

#---------------------------------------------------------------------

#mem_info_file(sys.stdout,0)

#mem_info_file(sys.stdout,MY_LIST1)
#mem_info_file(sys.stdout,MY_LIST2)
#mem_info_file(sys.stdout,MY_LIST3)
#mem_info_file(sys.stdout,MY_LIST4)
#mem_info_file(sys.stdout,MY_LIST5)
#mem_info_file(sys.stdout,MY_LIST6)
#mem_info_file(sys.stdout,MY_LIST7)
