#! /usr/bin/env python

import sys
import time

from meschach import *
from meschach_threaded import *



DO_IN_PROD  = True
DO_V_ADD    = True
DO_V_SUB    = True
DO_SV_MLT   = True
DO_V_MLTADD = True

LOOP = 10

# test in_prod & in_prod_threaded
# -------------------------------
if DO_IN_PROD:

    DIM = 10000000

    v1 = v_get(DIM)
    v2 = v_get(DIM)

    v1 = v_ones(v1)
    v2 = v_ones(v2)

    print("-----------------------")
    print("start in_prod ...")

    t1 = time.time()

    for i in range(LOOP):
        ip1 = in_prod(v1, v2)
        #print("NO THREAD: in_prod = %lf" % ip1)

    t2 = time.time()

    print("time (in_prod) =", t2-t1)

    # ----------------------------------------

    t1 = time.time()

    for i in range(LOOP):
        ip1 = in_prod_threaded(v1, v2)
        #print("WITH THREAD: in_prod_threaded = %lf" % ip1)

    t2 = time.time()

    print("time (in_prod_threaded) =", t2-t1)

    V_FREE(v1)
    V_FREE(v2)


# test v_add & v_add_threaded
# ---------------------------
if DO_V_ADD:

    DIM = 8000000

    v1 = v_get(DIM)
    v2 = v_get(DIM)
    v3 = v_get(DIM)

    v1 = v_ones(v1)
    v2 = v_ones(v2)

    sv_mlt(2.0, v1, v1)
    sv_mlt(3.0, v2, v2)

    print("-----------------------")
    print("start v_add ...")

    t1 = time.time()

    for i in range(LOOP):
        v_add(v1, v2, v3)

    t2 = time.time()

    print("time (v_add) =", t2-t1)

    # ----------------------------------------

    t1 = time.time()

    for i in range(LOOP):
        v_add_threaded(v1,v2,v3)

    t2 = time.time()

    print("time (v_add_threaded) =", t2-t1)

    V_FREE(v1)
    V_FREE(v2)
    V_FREE(v3)


# test v_sub & v_sub_threaded
# ---------------------------
if DO_V_SUB:

    DIM = 800000

    v1 = v_get(DIM)
    v2 = v_get(DIM)
    v3 = v_get(DIM)

    v1 = v_ones(v1)
    v2 = v_ones(v2)

    sv_mlt(2.0, v1, v1)
    sv_mlt(3.0, v2, v2)

    print("start v_sub ...")

    t1 = time.time()

    for i in range(LOOP):
        v_sub(v1,v2,v3)

    t2 = time.time()

    print("time (v_add) =", t2-t1)

    # ----------------------------------------

    t1 = time.time()

    for i in range(50):
        v_sub_threaded(v1,v2,v3)

    t2 = time.time()

    print("time (v_sub_threaded) =", t2-t1)

    # ----------------------------------------

    V_FREE(v1)
    V_FREE(v2)
    V_FREE(v3)


# test sv_mlt & sv_mlt_threaded
# ---------------------------
if DO_SV_MLT:

    DIM = 2000000

    v1 = v_get(DIM)
    v2 = v_get(DIM)

    v1 = v_ones(v1)
    v2 = v_ones(v2)

    print("-----------------------")
    print("start sv_mlt ...")

    t1 = time.time()

    for i in range(50):
        sv_mlt(2.5,v1,v2)

    #v_foutput(sys.stdout, v2)
    t2 = time.time()

    print("time (sv_mlt) =", t2-t1)

    # ----------------------------------------

    t1 = time.time()

    for i in range(50):
        sv_mlt_threaded(2.5,v1,v2)

        #v_foutput(sys.stdout, v2)
    t2 = time.time()

    print("time (sv_mlt_threaded) =", t2-t1)

    V_FREE(v1)
    V_FREE(v2)


# test sv_mltadd & sv_mltadd_threaded
# ---------------------------
if DO_V_MLTADD:

    DIM = 2000000

    v1 = v_get(DIM)
    v2 = v_get(DIM)
    v3 = v_get(DIM)

    v1 = v_ones(v1)
    v2 = v_ones(v2)


    print("-----------------------")
    print("start sv_mltadd ...")

    t1 = time.time()

    for i in range(50):
        v_mltadd(v1,v2,2.5,v3)
        #v_foutput(sys.stdout, v3)
    t2 = time.time()

    print("time (sv_mltadd) =", t2-t1)

    # ----------------------------------------

    t1 = time.time()

    for i in range(50):
        v_mltadd_threaded(v1,v2,2.5,v3)
        #v_foutput(sys.stdout, v3)

    t2 = time.time()

    print("time (sv_mltadd_threaded) =", t2-t1)

    V_FREE(v1)
    V_FREE(v2)
    V_FREE(v3)

