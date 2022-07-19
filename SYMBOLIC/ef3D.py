
import sys
import time

import ef3D_P1
import ef3D_P2
import ef3D_P3

# ---------------------------------------------------------------------------------

if __name__ == '__main__':
    '''
    '''
    args = sys.argv

    if len(sys.argv) != 2:
        print("usage:")
        print("       python ef3D.py <arg>")
        print("           with <arg> = P1 or P2 or P3")
    
        exit(0)



    t1 = time.time()

    if "P1" == sys.argv[1]:
    
        ef_P1 = ef3D_P1.ef3D_P1()
    
        ef_P1.testFunBase()
    
        ef_P1.bf_cdef()
        ef_P1.mx_cdef()


    if "P2" == sys.argv[1]:
    
        ef_P2 = ef3D_P2.ef3D_P2()
    
        ef_P2.testFunBase()
    
        ef_P2.bf_cdef()
        ef_P2.mx_cdef()


    if "P3" == sys.argv[1]:
    
        ef_P3 = ef3D_P3.ef3D_P3()
    
        #    ef_P3.testFunBase()
    
        #    ef_P3.bf_cdef()
        ef_P3.mx_cdef()


    t2 = time.time()

    print("Done (%d s. = %d min.)" % ((t2 - t1), (t2-t1)/60) )
