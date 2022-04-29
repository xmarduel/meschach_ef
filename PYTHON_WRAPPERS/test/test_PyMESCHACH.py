#! /usr/bin/env python
#
""" This module tests all the methods of the Meschach library (PyMESCHACH.py) """
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2004/07/24 14:36:25 $
#  $Revision: 1.6 $
#  ----------------------------------------------------------------
#
import unittest
from sys import *
#
from  PyMESCHACH import *
#
#
class MeschachTest(unittest.TestCase):
#
    #def test_CHsolve(self):
    def test_CHfactor(self):
        #
        DIM = 5
        #
        m1 = Mat(DIM, DIM)
        m2 = Mat(DIM, DIM)
        #
        m1.ones()
        m2.identity()
        #
        A = m1 + 5*m2
        #
        v1 = Vec(DIM)
        v1.ones()
        #
        v2 = A*v1
        #
        #print A
        CH = CHfactor(A)
        #print CH
        #
        v3 = CHsolve(CH,v2)
        #
        v_diff = v3-v1
        #
        diff = v_diff.norm()
        #
        if ( abs(diff) < 1.0e-8 ):
            diff = 0.0

        self.assertEqual(diff, 0.0)
        #
    #
    #
    #
    #
#
#
#
if __name__ == '__main__' :
    unittest.main()
#
