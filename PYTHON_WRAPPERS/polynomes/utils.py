#
#
from math import *
#
# build the assemblage matrices for element P1-P2-P3-H3-S3-S5 in DIM 1
#
#
#
def rounding_value(val, code):
    """
    a = 1.9999 -> aa = 2.0  (ceil(a))
    a = 2.0001 -> aa = 2.0  (floor(a))
    
    b = -2.999 -> bb = -3.0    (ceil(a))
    b = -3.0001 -> bb = -3.0   (floor(a))
    
    c = 2.456 -> ERROR
    """
    val1 = floor(val)
    val2 = ceil(val)
    #
    tol = 0.01
    # check val is nearly an integer
    if ( not( ((val-val1)< tol) or ((val-val2)< tol) ) ):
       print "ERROR............................", code, val
       exit(0)    
    
    #
    val_ok = 0.0
    #
    if ( abs(val-val1) < abs(val-val2) ):
        #
        val_ok = val1
    else:
        val_ok = val2
    #
    #print "val = ", val, "(",code,")  -> val_ok = ", val_ok
    #
    return val_ok
#
#