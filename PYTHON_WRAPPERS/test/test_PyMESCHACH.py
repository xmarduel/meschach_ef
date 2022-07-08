#! /usr/bin/env python

""" This module tests all the methods of the Meschach library (PyMESCHACH.py) """

import sys
import unittest

import PyMESCHACH

class MeschachTest(unittest.TestCase):

    def test_CHfactor(self):
        DIM = 5

        m1 = PyMESCHACH.Mat(DIM, DIM)
        m2 = PyMESCHACH.Mat(DIM, DIM)

        m1.ones()
        m2.identity()

        A = m1 + DIM*m2

        v1 = PyMESCHACH.Vec(DIM)
        v1.ones()

        v2 = A*v1

        print("v1 = %s" % v1)
        print("A*v1 = v2 = %s" % v2)

        #print(A)
        CH = PyMESCHACH.CHfactor(A)
        #print(CH)

        v3 = PyMESCHACH.CHsolve(CH, v2)

        print("v3 sol of inv(A)(v2)= %s" % v3)

        v_diff = v3 - v1

        diff = v_diff.norm()

        print("diff = ", diff)
        if ( abs(diff) < 1.0e-8 ):
            diff = 0.0

        self.assertEqual(diff, 0.0)



if __name__ == '__main__' :
    unittest.main()
