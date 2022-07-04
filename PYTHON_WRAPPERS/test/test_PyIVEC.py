#! /usr/bin/env python

""" This module tests all the methods of the IVec class (PyIVEC.py) """

import unittest
import PyIVEC


class IVecTest(unittest.TestCase):

    def testInit(self):
        m1 =  PyIVEC.IVec(3)
        m2 =  PyIVEC.IVec(0)

    def test___del__(self):
        m1 =  PyIVEC.IVec(3)
        del m1

    #
    # Getting the data
    #
    def test___len__(self):
        N = 10
        v1 = PyIVEC.IVec(N)
        self.assertEqual(len(v1), N)

    def test___setitem__(self):
        v1 = PyIVEC.IVec(3)
        v1.ones()
        v1[1] = 33
        self.assertEqual(v1.__str__(), 'IntVector: dim: 3\n       1       33        1 \n')
        self.assertRaises(TypeError, 'v1[1] = 33.3')

    def test___getitem__(self):
        val = 33
        v1 = PyIVEC.IVec(3)
        v1.ones()
        v1[1] = val
        self.assertEqual(v1[1], val)

    #
    # Input - Output diagnostics
    #
    #def test_out(self):
    #
    #def test_dump(self):
    #
    #def test___repr__(self):
    #

    def test___str__(self):
        v1 = PyIVEC.IVec(3)
        self.assertEqual(v1.__str__(), 'IntVector: dim: 3\n       0        0        0 \n')

    #
    # playing with storage
    #

    #def free(self):
    #    v_free(self.this)
    #    self.this = v_null()
    #
    #def resize(self,m):
    #    v_resize(self.this, m)
    #
    #
    # Members functions
    #
    def test_ones(self):
        v1 = PyIVEC.IVec(3)
        v1.ones()
        self.assertEqual(v1.__str__(), 'IntVector: dim: 3\n       1        1        1 \n')

    def test_zero(self):
        v1 = PyIVEC.IVec(3)
        v1.zero()
        self.assertEqual(v1.__str__(), 'IntVector: dim: 3\n       0        0        0 \n')

    #def rand(self):
    #    v_rand(self.this)


    #
    # operators creating temporary
    #
    def test___add__(self):
        v1 = PyIVEC.IVec(3)
        v2 = PyIVEC.IVec(3)
        # a is a Vec
        v1.ones()
        v2.ones()
        v3 = v1 + v2
        self.assertEqual(v3.__str__(), 'IntVector: dim: 3\n       2        2        2 \n')
        v2 = v1 + 5
        self.assertEqual(v2.__str__(), 'IntVector: dim: 3\n       6        6        6 \n')
        v2 = 6 + v1
        self.assertEqual(v2.__str__(), 'IntVector: dim: 3\n       7        7        7 \n')

    def test___sub__(self):
        v1 = PyIVEC.IVec(3)
        v2 = PyIVEC.IVec(3)
        # a is a Vec
        v1.ones()
        v2.ones()
        v3 = v1 - v2
        self.assertEqual(v3.__str__(), 'IntVector: dim: 3\n       0        0        0 \n')
        v2 = v1 - 1
        self.assertEqual(v2.__str__(), 'IntVector: dim: 3\n       0        0        0 \n')
        v2 = 1 - v1
        self.assertEqual(v2.__str__(), 'IntVector: dim: 3\n       0        0        0 \n')
    #
    #
    #def test___mul__1(self):
    #    v1 = PyIVEC.IVec(3)
    #    v2 = PyIVEC.IVec(3)
    #    # a is a Vec
    #    v1.ones()
    #    v2.ones()
    #    v3 = v1 * v2
    #    self.assertEqual(v3.__str__(), '3.0')
    #
    def test___mul__(self):
        v1 = PyIVEC.IVec(3)
        # a is a Vec
        v1.ones()
        v2 = v1*2
        self.assertEqual(v2.__str__(), 'IntVector: dim: 3\n       2        2        2 \n')
        v2 = 2*v1
        self.assertEqual(v2.__str__(), 'IntVector: dim: 3\n       2        2        2 \n')

    #
    # operator using and returning base object
    #
    def test___iadd__(self):
        v1 = PyIVEC.IVec(3)
        v2 = PyIVEC.IVec(3)
        # a is a Vec
        v1.ones()
        v2.ones()
        v1 += v2
        self.assertEqual(v1.__str__(), 'IntVector: dim: 3\n       2        2        2 \n')
        v1 += 1
        self.assertEqual(v1.__str__(), 'IntVector: dim: 3\n       3        3        3 \n')

    def test___isub__(self):
        v1 = PyIVEC.IVec(3)
        v2 = PyIVEC.IVec(3)
        # a is a Vec
        v1.ones()
        v2.ones()
        v1 -= v2
        self.assertEqual(v1.__str__(), 'IntVector: dim: 3\n       0        0        0 \n')
        v1 -= 1
        self.assertEqual(v1.__str__(), 'IntVector: dim: 3\n      -1       -1       -1 \n')

    def test___imul__(self):
        v1 = PyIVEC.IVec(3)
        # a is a Vec
        v1.ones()
        v1 *= 5
        self.assertEqual(v1.__str__(), 'IntVector: dim: 3\n       5        5        5 \n')

    #
    # A = + B
    def test___pos__(self):
        v1 = PyIVEC.IVec(3)
        # a is a Vec
        v1.ones()
        v2 = + v1
        v1 *= 2 # change v1
        self.assertEqual(v2.__str__(), 'IntVector: dim: 3\n       1        1        1 \n')

    # A = - B
    def test___neg__(self):
        v1 = PyIVEC.IVec(3)
        # a is a Vec
        v1.ones()
        v2 = - v1
        v1 *= 2 # change v1
        self.assertEqual(v2.__str__(), 'IntVector: dim: 3\n      -1       -1       -1 \n')

#
#

if __name__ == '__main__' :
    unittest.main()
