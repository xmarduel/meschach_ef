#! /usr/bin/env python
#
""" This module tests all the methods of the SpMat class (PySPMAT.py) """
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2004/07/24 14:32:23 $
#  $Revision: 1.7 $
#  ----------------------------------------------------------------
#
import unittest
import PySPMAT
#
#
#
#
class SpMatTest(unittest.TestCase):
    #
    #
    def testInit(self): 
        #
        m1 = PySPMAT.SpMat(4,4)
        #
        try: m2 = PySPMAT.SpMat(-4,4)
        except IndexError:
            pass
        try: m3 = PySPMAT.SpMat(4,-4)
        except IndexError:
            pass               
    #
    #
    def test___del__(self):
        #
        m1 = PySPMAT.SpMat(4,4)
        del m1
    #
    #
    #
    # Getting the data
    #
    def test_property_m(self):
        M = 5
        m1 = PySPMAT.SpMat(M,4)
        self.assertEqual(m1.m, M)
    #
    def test_property_n(self):
        N = 5
        m1 = PySPMAT.SpMat(4,N)
        self.assertEqual(m1.n, N)
    #
    #
    def test___setitem__(self):
        m1 = PySPMAT.SpMat(3,2)
        #
        m1.ones()
        m1[1][1] = 33
        self.assertEqual(m1.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:1                    1:1                    \nrow 1: 0:1                    1:33                   \nrow 2: 0:1                    1:1                    \n#\n')
    #
    def test___getitem__(self):
        m1 = PySPMAT.SpMat(3,2)
        #
        m1.ones()
        val = 33.3
        m1[1][1] = val
        self.assertEqual(m1[1][1], val)
    #
    #
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
        m1 = PySPMAT.SpMat(3,2)
        self.assertEqual(m1.__str__(), 'SparseMatrix: 3 by 2\nrow 0: \nrow 1: \nrow 2: \n#\n')    #
    #
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
        m1 = PySPMAT.SpMat(3,2)
        m1.ones()
        self.assertEqual(m1.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:1                    1:1                    \nrow 1: 0:1                    1:1                    \nrow 2: 0:1                    1:1                    \n#\n')
    #
    def test_zero(self):
        m1 = PySPMAT.SpMat(3,2)
        m1.ones()
        m1.zero()
        self.assertEqual(m1.__str__(), 'SparseMatrix: 3 by 2\nrow 0: \nrow 1: \nrow 2: \n#\n')
    #
    def test_eye(self):
        m1 = PySPMAT.SpMat(2,2)
        m1.ones()
        m1.eye()
        self.assertEqual(m1.__str__(), 'SparseMatrix: 2 by 2\nrow 0: 0:1                    \nrow 1: 1:1                    \n#\n')
    #
    def test_tridiag(self):
        m1 = PySPMAT.SpMat(3,3)
        m1.tridiag(-1,4,-1)
        self.assertEqual(m1.__str__(), 'SparseMatrix: 3 by 3\nrow 0: 0:4                    1:-1                   \nrow 1: 0:-1                   1:4                    2:-1                   \nrow 2: 1:-1                   2:4                    \n#\n')
    #
    #def rand(self):
    #    v_rand(self.this)
    #
    # 
    #
    # operators creating temporary
    #
    def test___add__(self):
        m1 = PySPMAT.SpMat(3,2)
        m2 = PySPMAT.SpMat(3,2)
        #
        m1.ones()
        m2.ones()
        m3 = m1 + m2
        self.assertEqual(m3.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:2                    1:2                    \nrow 1: 0:2                    1:2                    \nrow 2: 0:2                    1:2                    \n#\n')
        m3 = m1 + m3
        self.assertEqual(m3.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:3                    1:3                    \nrow 1: 0:3                    1:3                    \nrow 2: 0:3                    1:3                    \n#\n')
    #
    #
    def test___sub__(self):
        m1 = PySPMAT.SpMat(3,2)
        m2 = PySPMAT.SpMat(3,2)
        #
        m1.ones()
        m2.ones()
        m3 = m1 - m2
        self.assertEqual(m3.__str__(), 'SparseMatrix: 3 by 2\nrow 0: \nrow 1: \nrow 2: \n#\n')
        m3 = m1 - m3
        self.assertEqual(m3.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:1                    1:1                    \nrow 1: 0:1                    1:1                    \nrow 2: 0:1                    1:1                    \n#\n')
    #
    #
    def test___mul__(self):
        m1 = PySPMAT.SpMat(3,2)
        m2 = PySPMAT.SpMat(2,2)
        #
        m1.ones()
        m2.ones()
        m3 = m1 * m2
        self.assertEqual(m3.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:2                    1:2                    \nrow 1: 0:2                    1:2                    \nrow 2: 0:2                    1:2                    \n#\n')
        m2 = m1*2
        self.assertEqual(m2.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:2                    1:2                    \nrow 1: 0:2                    1:2                    \nrow 2: 0:2                    1:2                    \n#\n')
        m2 = 2*m2
        self.assertEqual(m2.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:4                    1:4                    \nrow 1: 0:4                    1:4                    \nrow 2: 0:4                    1:4                    \n#\n')
    #
    #
    #
    # operator using and returning base object
    #
    def test___iadd__(self):
        m1 = PySPMAT.SpMat(3,2)
        m2 = PySPMAT.SpMat(3,2)
        #
        m1.ones()
        m2.ones()
        m1 += m2
        self.assertEqual(m1.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:2                    1:2                    \nrow 1: 0:2                    1:2                    \nrow 2: 0:2                    1:2                    \n#\n')
    #
    #
    def test___isub__(self):
        m1 = PySPMAT.SpMat(3,2)
        m2 = PySPMAT.SpMat(3,2)
        #
        m1.ones()
        m2.ones()
        m1 -= m2
        self.assertEqual(m1.__str__(), 'SparseMatrix: 3 by 2\nrow 0: \nrow 1: \nrow 2: \n#\n')
    #
    #
    def test___imul__(self):
        m1 = PySPMAT.SpMat(3,2)
        m2 = PySPMAT.SpMat(2,2)
        #
        m1.ones()
        m2.ones()
        m1 *= 3
        self.assertEqual(m1.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:3                    1:3                    \nrow 1: 0:3                    1:3                    \nrow 2: 0:3                    1:3                    \n#\n')
        m1 *= m2
        self.assertEqual(m1.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:6                    1:6                    \nrow 1: 0:6                    1:6                    \nrow 2: 0:6                    1:6                    \n#\n')
    #
    #
    #
    # assignation operator : with __pos__ 
    #
    # A = + B
    def test___pos__(self):
        m1 = PySPMAT.SpMat(3,2)
        #
        m1.ones()
        m2 = + m1
        m1 *= 2 # change m1
        self.assertEqual(m2.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:1                    1:1                    \nrow 1: 0:1                    1:1                    \nrow 2: 0:1                    1:1                    \n#\n')
    #
    # A = - B
    def test___neg__(self):
        m1 = PySPMAT.SpMat(3,2)
        #
        m1.ones()
        m2 = - m1
        m1 *= 2 # change m1
        self.assertEqual(m2.__str__(), 'SparseMatrix: 3 by 2\nrow 0: 0:-1                   1:-1                   \nrow 1: 0:-1                   1:-1                   \nrow 2: 0:-1                   1:-1                   \n#\n')
    #
    #
    #
#
#
#
if __name__ == '__main__' :
    unittest.main()
#
#
