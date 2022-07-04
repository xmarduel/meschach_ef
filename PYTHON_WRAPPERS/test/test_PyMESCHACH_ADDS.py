#! /usr/bin/env python

""" This module tests all the methods of the Meschach_adds library (PyMESCHACH.py) """

import sys
import unittest

import  PyMESCHACH

import libmeschach
import libmeschach_adds

class MeschachAddsTest(unittest.TestCase):

    def test_riccati(self):

        DIM = 4

        A = PyMESCHACH.Mat(4,4)
        B = PyMESCHACH.Mat(4,4)
        C = PyMESCHACH.Mat(4,4)
        X = PyMESCHACH.Mat(4,4)

        # set A, B and X
        A.identity()
        B.identity()
        X.ones()

        for i in range(DIM):
            for j in range(DIM):
                A[i][j] = i+j
                B[i][j] = 2*i+1*j+2

        for i in range(DIM): # make symmetric
            for j in range(i):
                A[i][j] = 2*A[j][i]   # A not symmetric
                B[i][j] = B[j][i]     # B symmetric

        for i in range(DIM):
            A[i][i] = -i*i

        At = ~A

        # with A, B and X, deduce C
        C =  - At*X - X*A + X*B*X
        #print "\nC with - At*X - X*A + X*B*X  "
        #C.out()
        #
        libmeschach_adds.set_control_method(libmeschach_adds.CONTROLe_SLICOT)
        #libmeschach_adds.set_control_method(libmeschach_adds.CONTROLe_SCILAB)
        # with A, B and C, find Y : it should be  equal to X ...
        Y = riccati(A, B, C)

        #print " we find Y :"
        #Y.out()
        #print "and retrieve C with - At*Y - Y*A + Y*B*Y"
        #CC = - At*Y - Y*A + Y*B*Y
        #CC.out()

        DIFF = X - Y

        diff = DIFF.norm_frob()
        if ( abs(diff) < 1.0e-8 ):
            diff = 0.0

        self.assertEquals(0.0 ,diff)

    def test_lyapunov(self):
        DIM = 4

        A = Mat(4,4)
        C = Mat(4,4)
        X = Mat(4,4)

        At = Mat(4,4)

        # set A, B and X
        A.ones()
        X.ones()

        A[0][0] = 4
        A[1][1] = 5
        A[2][2] = 6
        A[3][3] = 7

        A[1][0] = 5 # A non symmetric

        At = ~A
        # with A, B and X, deduce C: C = A'.X + X.A
        C = At*X + X*A
        #print "C is  "
        #C.out()
        #
        libmeschach_adds.set_control_method(libmeschach_adds.CONTROLe_SLICOT)
        #libmeschach_adds.set_control_method(libmeschach_adds.CONTROLe_SCILAB)
        # with A and C, find Y : it should be  equal to X ...
        Y = lyapunov(A, C)
        #
        #print " we find Y :"
        #Y.out()
        #print "and retrieve C ... "
        #CC = At*Y + Y*A
        #CC.out()
        #
        DIFF = X - Y
        #
        diff = DIFF.norm_frob()
        if ( abs(diff) < 1.0e-8 ):
            diff = 0.0
        #
        self.assertEquals(0.0 ,diff)
        #
        #
    #
    def test_sylvester(self):
        #
        DIM = 4
        #
        A = Mat(4,4)
        B = Mat(4,4)
        C = Mat(4,4)
        X = Mat(4,4)
        #
        # set A, B and X
        A.ones()
        B.ones()
        X.ones()

        A[0][0] = 4
        A[1][1] = 5
        A[2][2] = 6
        A[3][3] = 7
        A *= 1.0

        B *= 2.0
        B[0][0] = 4
        B[1][1] = 5
        B[2][2] = 6
        B[3][3] = 7

        # with A, B and X, deduce C
        C = A*X + X*B

        libmeschach_adds.set_control_method(libmeschach_adds.CONTROLe_SLICOT)
        #libmeschach_adds.set_control_method(libmeschach_adds.CONTROLe_SCILAB)
        # with A, B and C, find Y : it should be  equal to X ...
        Y = sylvester(A, B, C)

        #print " we find Y :"
        #Y.out()
        #print "and retrieve C ... "
        #CC = A*Y + Y*B
        #CC.out()

        DIFF = X - Y

        diff = DIFF.norm_frob()
        if ( abs(diff) < 1.0e-8 ):
            diff = 0.0

        self.assertEquals(0.0 ,diff)



if __name__ == '__main__' :
    unittest.main()

