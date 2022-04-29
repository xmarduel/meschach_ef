#
""" This module tests all the methods of the Poly1D class (Pol1D.py) """
#
import unittest
import Pol1D
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2005/10/30 12:59:13 $
#  $Revision: 1.2 $
#  ----------------------------------------------------------------
#
#
class Poly1DTest(unittest.TestCase):
    #
    #
    # Input - Output diagnostics
    #
    def test___repr__(self):
        p1 = Pol1D.Poly1D("1 + 2*X + 3*X*X + 4*X*X*X")
        self.assertEqual(p1.__repr__(), ' +1.000000 +2.000000*X +3.000000*X*X +4.000000*X*X*X')
        p1 = Pol1D.Poly1D("0")
        self.assertEqual(p1.__repr__(), ' +0.000000')
        p1 = Pol1D.Poly1D("X")
        self.assertEqual(p1.__repr__(), ' +1.000000*X')
    #
    #
    def test___str__(self):
        p1 = Pol1D. Poly1D("1 + 2*X + 3*X*X + 4*X*X*X")
        self.assertEqual(p1.__str__(), ' +1.000000 +2.000000*X +3.000000*X*X +4.000000*X*X*X')
        p1 = Pol1D.Poly1D("0")
        self.assertEqual(p1.__str__(), ' +0.000000')
        p1 = Pol1D.Poly1D("X")
        self.assertEqual(p1.__repr__(), ' +1.000000*X')
    #	
    #
    # Members functions
    #
    # operators creating temporary
    #
    def test___add__(self):
        p1 = Pol1D.Poly1D("1 + 2*X + 4*X*X*X")
        p2 = Pol1D.Poly1D("3*X*X")
        p3 = Pol1D.Poly1D("-3 - 3*X - 3*X*X - 4*X*X*X")
        p4 = Pol1D.Poly1D("-1 - 2*X - 4*X*X*X")
        #
        pp1 = p1 + p2
        self.assertEqual(pp1.__str__(), ' +1.000000 +2.000000*X +3.000000*X*X +4.000000*X*X*X')
        pp2 = p1 + p3
        self.assertEqual(pp2.__str__(), ' -2.000000 -1.000000*X -3.000000*X*X')
        pp3 = p1 + p4
        self.assertEqual(pp3.__str__(), ' 0')
    #
    #
    def test___sub__(self):
        p1 = Pol1D.Poly1D("1 + 2*X + 3*X*X + 4*X*X*X")
        p2 = Pol1D.Poly1D("1 + 2*X + 3*X*X + 4*X*X*X")
        p3 = Pol1D.Poly1D("-1 - 2*X + 3*X*X + 4*X*X*X")
        p4 = Pol1D.Poly1D("-1 - 2*X - 3*X*X - 4*X*X*X")
        #
        pp1 = p1 - p2
        self.assertEqual(pp1.__str__(), ' 0')
        pp2 = p1 - p3
        self.assertEqual(pp2.__str__(), ' +2.000000 +4.000000*X')
        pp3 = p1 - p4
        self.assertEqual(pp3.__str__(), ' +2.000000 +4.000000*X +6.000000*X*X +8.000000*X*X*X')
    #
    #
    def test___mul__(self):
        p1 = Pol1D.Poly1D("1 + 2*X + 3*X*X + 4*X*X*X")
        p2 = Pol1D.Poly1D("-2")
        p3 = Pol1D.Poly1D("X")
        p4 = Pol1D.Poly1D("0")
        #
        pp1 = p1 * p2
        self.assertEqual(pp1.__str__(), ' -2.000000 -4.000000*X -6.000000*X*X -8.000000*X*X*X')
        pp2 = p1 * p3
        self.assertEqual(pp2.__str__(), ' +1.000000*X +2.000000*X*X +3.000000*X*X*X +4.000000*X*X*X*X')
        pp3 = p2 * p4
        self.assertEqual(pp3.__str__(), ' +0.000000')
    #
    #
    #
    # assignation operator : with __pos__ 
    #
    # A = + B
    def test___pos__(self):
        p1 = Pol1D.Poly1D("1 + 2*X + 3*X*X + 4*X*X*X")
        #
        p2 = + p1
        p1 = Pol1D.Poly1D("0") # change p1
        self.assertEqual(p2.__str__(), ' +1.000000 +2.000000*X +3.000000*X*X +4.000000*X*X*X')
    #
    # A = - B
    def test___neg__(self):
        p1 = Pol1D.Poly1D("1 + 2*X + 3*X*X + 4*X*X*X")
        #
        p2 = - p1
        p1 = Pol1D.Poly1D("0") # change p1
        self.assertEqual(p2.__str__(), ' -1.000000 -2.000000*X -3.000000*X*X -4.000000*X*X*X')
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
