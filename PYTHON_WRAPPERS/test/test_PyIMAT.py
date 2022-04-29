#! /usr/bin/env python
#
""" This module tests all the methods of the IMat class (PyIMAT.py) """
#
import unittest
import PyIMAT
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2004/10/01 06:20:06 $
#  $Revision: 1.10 $
#
#  $Log: test_PyIMAT.py,v $
#  Revision 1.10  2004/10/01 06:20:06  xavier
#  No log message.
#
#  Revision 1.9  2004/07/30 06:52:00  xavier
#  update
#
#  Revision 1.8  2004/07/24 14:32:23  xavier
#  No log message.
#
#  Revision 1.7  2004/07/22 11:46:38  xavier
#  No log message.
#
#  Revision 1.6  2004/07/04 17:00:21  xavier
#  continues to implement ...
#
#  Revision 1.5  2004/07/04 10:01:36  xavier
#  update
#
#  ----------------------------------------------------------------
#
#
class IMatTest(unittest.TestCase):
    #
    #
    def testInit(self): 
    #
        m1 =  PyIMAT.IMat(3,3)
        print "\ntry IMat(-1,4) (to raise an exception that will be caught)"
        try: m3 = PyIMAT.IMat(-1,4)
        except IndexError:
            pass 
        #
        print "try IMat(4) (to raise an exception that will be caught)"
        try: m4 = PyIMAT.IMat(4)
        except TypeError:
            pass
            #print " detected <TypeError>, it is OK ..."
    #
    #
    def test___del__(self):
        #
        m1 =  PyIMAT.IMat(3,3)
        del m1
    #
    #
    def test_resize(self):
        m1 =  PyIMAT.IMat(3,3)
        m1.ones()
        m1.resize(3,2)
        m1.ones()
        self.assertEqual(m1.__str__(), 'IntMatrix: 3 by 2\nrow 0:      1      1 \nrow 1:      1      1 \nrow 2:      1      1 \n')
    #
    # Getting the data
    #
    def test_property_m(self):
        M = 5
        m1 = PyIMAT.IMat(M,4)
        self.assertEqual(m1.m, M)
    #
    def test_property_n(self):
        N = 5
        m1 = PyIMAT.IMat(4,N)
        self.assertEqual(m1.n, N)
    #
    #
    def test___setitem__(self):
        m1 = PyIMAT.IMat(3,2)
        #
        m1.ones()
        m1[1][1] = 33
        self.assertEqual(m1.__str__(), 'IntMatrix: 3 by 2\nrow 0:      1      1 \nrow 1:      1     33 \nrow 2:      1      1 \n')
    #
    def test___getitem__(self):
        m1 = PyIMAT.IMat(3,2)
        #
        m1.ones()
        val = 33
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
        m1 = PyIMAT.IMat(3,2)
        self.assertEqual(m1.__str__(), 'IntMatrix: 3 by 2\nrow 0:      0      0 \nrow 1:      0      0 \nrow 2:      0      0 \n')
    #
    #
    #	
    #
    # Members functions
    #
    def test_ones(self):
        m1 = PyIMAT.IMat(3,2)
        m1.ones()
        self.assertEqual(m1.__str__(), 'IntMatrix: 3 by 2\nrow 0:      1      1 \nrow 1:      1      1 \nrow 2:      1      1 \n')
    #
    def test_zero(self):
        m1 = PyIMAT.IMat(3,2)
        m1.ones()
        m1.zero()
        self.assertEqual(m1.__str__(), 'IntMatrix: 3 by 2\nrow 0:      0      0 \nrow 1:      0      0 \nrow 2:      0      0 \n')
    #
    #def rand(self):
    #    v_rand(self.this)
    #
    # 
    #
    # transpose operator
    def test___invert__(self):
        m1 = PyIMAT.IMat(5, 3)
        m1.rand()
        m2 = ~m1
        #
        is_ok = True
        # check
        for i in range(5):
            for j in range(3):
                if ( abs( m1[i][j] - m2[j][i] ) > 1.0e-8 ):
                    is_ok = False
                    break
        #
        self.assertEqual(is_ok, True)
    #
    #
    #
    # operators creating temporary
    #
    def test___add__(self):
        m1 = PyIMAT.IMat(3,2)
        m2 = PyIMAT.IMat(3,2)
        #
        m1.ones()
        m2.ones()
        m3 = m1 + m2
        self.assertEqual(m3.__str__(), 'IntMatrix: 3 by 2\nrow 0:      2      2 \nrow 1:      2      2 \nrow 2:      2      2 \n')
        m2 = m1 + 5
        self.assertEqual(m2.__str__(), 'IntMatrix: 3 by 2\nrow 0:      6      6 \nrow 1:      6      6 \nrow 2:      6      6 \n')
        m2 = 3 + m1
        self.assertEqual(m2.__str__(), 'IntMatrix: 3 by 2\nrow 0:      4      4 \nrow 1:      4      4 \nrow 2:      4      4 \n')
        m2 = 3 + m2
        self.assertEqual(m2.__str__(), 'IntMatrix: 3 by 2\nrow 0:      7      7 \nrow 1:      7      7 \nrow 2:      7      7 \n')
    #
    #
    def test___sub__(self):
        m1 = PyIMAT.IMat(3,2)
        m2 = PyIMAT.IMat(3,2)
        #
        m1.ones()
        m2.ones()
        m3 = m1 - m2
        self.assertEqual(m3.__str__(), 'IntMatrix: 3 by 2\nrow 0:      0      0 \nrow 1:      0      0 \nrow 2:      0      0 \n')
        m2 = m1 - 1
        self.assertEqual(m2.__str__(), 'IntMatrix: 3 by 2\nrow 0:      0      0 \nrow 1:      0      0 \nrow 2:      0      0 \n')
        m2 = 1 - m1
        self.assertEqual(m2.__str__(), 'IntMatrix: 3 by 2\nrow 0:      0      0 \nrow 1:      0      0 \nrow 2:      0      0 \n')
    #
    #
    def test___mul__(self):
        m1 = PyIMAT.IMat(3,2)
        m2 = PyIMAT.IMat(2,2)
        #
        m1.ones()
        m2.ones()
        m3 = m1 * m2
        self.assertEqual(m3.__str__(), 'IntMatrix: 3 by 2\nrow 0:      2      2 \nrow 1:      2      2 \nrow 2:      2      2 \n')
        m2 = m1*2
        self.assertEqual(m2.__str__(), 'IntMatrix: 3 by 2\nrow 0:      2      2 \nrow 1:      2      2 \nrow 2:      2      2 \n')
        m2 = 2*m2
        self.assertEqual(m2.__str__(), 'IntMatrix: 3 by 2\nrow 0:      4      4 \nrow 1:      4      4 \nrow 2:      4      4 \n')
    #
    #
    #
    # operator using and returning base object
    #
    def test___iadd__(self):
        m1 = PyIMAT.IMat(3,2)
        m2 = PyIMAT.IMat(3,2)
        #
        m1.ones()
        m2.ones()
        m1 += m2
        self.assertEqual(m1.__str__(), 'IntMatrix: 3 by 2\nrow 0:      2      2 \nrow 1:      2      2 \nrow 2:      2      2 \n')
        m1 += 1
        self.assertEqual(m1.__str__(), 'IntMatrix: 3 by 2\nrow 0:      3      3 \nrow 1:      3      3 \nrow 2:      3      3 \n')
    #
    #
    def test___isub__(self):
        m1 = PyIMAT.IMat(3,2)
        m2 = PyIMAT.IMat(3,2)
        #
        m1.ones()
        m2.ones()
        m1 -= m2
        self.assertEqual(m1.__str__(), 'IntMatrix: 3 by 2\nrow 0:      0      0 \nrow 1:      0      0 \nrow 2:      0      0 \n')
        m1 -= 1
        self.assertEqual(m1.__str__(), 'IntMatrix: 3 by 2\nrow 0:     -1     -1 \nrow 1:     -1     -1 \nrow 2:     -1     -1 \n')
    #
    #
    def test___imul__(self):
        m1 = PyIMAT.IMat(3,2)
        m2 = PyIMAT.IMat(2,2)
        #
        m1.ones()
        m2.ones()
        m1 *= 3
        self.assertEqual(m1.__str__(), 'IntMatrix: 3 by 2\nrow 0:      3      3 \nrow 1:      3      3 \nrow 2:      3      3 \n')
        m1 *= m2
        self.assertEqual(m1.__str__(), 'IntMatrix: 3 by 2\nrow 0:      6      6 \nrow 1:      6      6 \nrow 2:      6      6 \n')
    #
    #
    #
    # assignation operator : with __pos__ 
    #
    # A = + B
    def test___pos__(self):
        m1 = PyIMAT.IMat(3,2)
        #
        m1.ones()
        m2 = + m1
        m1 *= 2 # change m1
        self.assertEqual(m2.__str__(), 'IntMatrix: 3 by 2\nrow 0:      1      1 \nrow 1:      1      1 \nrow 2:      1      1 \n')
    #
    # A = - B
    def test___neg__(self):
        m1 = PyIMAT.IMat(3,2)
        #
        m1.ones()
        m2 = - m1
        m1 *= 2 # change m1
        self.assertEqual(m2.__str__(), 'IntMatrix: 3 by 2\nrow 0:     -1     -1 \nrow 1:     -1     -1 \nrow 2:     -1     -1 \n')
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
#'Matrix: 3 by 2\nrow 0:      0      0 \nrow 1:      0      0 \nrow 2:      0      0 \n' != 
#'Matrix: 3 by 2\nrow 0:      0      0 \nrow 1:      0      0 \nrow 2:      0      0 \n'
