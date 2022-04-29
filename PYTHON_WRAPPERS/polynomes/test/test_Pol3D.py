#
""" This module tests all the methods of the Poly3D class (Pol3D.py) """
#
import unittest
import Pol3D
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2004/01/31 17:19:31 $
#  $Revision: 1.1 $
#
#  ----------------------------------------------------------------
#
#
class Poly3DTest(unittest.TestCase):
    #
    #
    #def testInit(self): 
    #    #
    #    #self.assertEqual(classVEC.__init__(3)),        '...')
    #    #
    #    #self.assertRaises(typeError, classVEC.Vec(-1))
    #    #               
    #
    #
    #
    #
    # Input - Output diagnostics
    #
    def test___repr__(self):
        p1 = Pol3D.Poly3D("1 + 2*X + 3*X*X + 4*X*X*X")
        self.assertEqual(p1.__repr__(), ' +1.000000 +2.000000*X +3.000000*X*X +4.000000*X*X*X')
        p1 = Pol3D.Poly3D("0")
        self.assertEqual(p1.__repr__(), ' +0.000000')
        p1 = Pol3D.Poly3D("X")
        self.assertEqual(p1.__repr__(), ' +1.000000*X')
    #
    #
    def test___str__(self):
        p1 = Pol3D. Poly3D("1 + 2*X + 3*X*X + 4*X*X*X")
        self.assertEqual(p1.__str__(), ' +1.000000 +2.000000*X +3.000000*X*X +4.000000*X*X*X')
        p1 = Pol3D.Poly3D("0")
        self.assertEqual(p1.__str__(), ' +0.000000')
        p1 = Pol3D.Poly3D("X")
        self.assertEqual(p1.__repr__(), ' +1.000000*X')
    #	
    #
    # Members functions
    #
    #def test_deg(self):
    #    p1 = Pol3D.Poly3D("1 + 2*X + 3*X*X + 4*X*X*X")
    #    self.assertEqual(p1.deg(), 3)
    #
    #
    # operators creating temporary
    #
    def test___add__(self):
        p1 = Pol3D.Poly3D("1 + 2*X + 4*X*X*X")
        p2 = Pol3D.Poly3D("3*X*X")
        p3 = Pol3D.Poly3D("-3 - 3*X - 3*X*X - 4*X*X*X")
        p4 = Pol3D.Poly3D("-1 - 2*X - 4*X*X*X")
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
        p1 = Pol3D.Poly3D("1 + 2*X + 3*X*X + 4*X*X*X")
        p2 = Pol3D.Poly3D("1 + 2*X + 3*X*X + 4*X*X*X")
        p3 = Pol3D.Poly3D("-1 - 2*X + 3*X*X + 4*X*X*X")
        p4 = Pol3D.Poly3D("-1 - 2*X - 3*X*X - 4*X*X*X")
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
        p1 = Pol3D.Poly3D("1 + 2*X + 3*X*X + 4*X*X*X")
        p2 = Pol3D.Poly3D("-2")
        p3 = Pol3D.Poly3D("0")
        #
        pp1 = p1 * p2
        self.assertEqual(pp1.__str__(), ' -2.000000 -4.000000*X -6.000000*X*X -8.000000*X*X*X')
        pp2 = p2 * p3
        self.assertEqual(pp2.__str__(), ' +0.000000')
    #
    #
    #
    # assignation operator : with __pos__ 
    #
    # A = + B
    def test___pos__(self):
        p1 = Pol3D.Poly3D("1 + 2*X + 3*X*X + 4*X*X*X")
        #
        p2 = + p1
        p1 = Pol3D.Poly3D("0") # change p1
        self.assertEqual(p2.__str__(), ' +1.000000 +2.000000*X +3.000000*X*X +4.000000*X*X*X')
    #
    # A = - B
    def test___neg__(self):
        p1 = Pol3D.Poly3D("1 + 2*X + 3*X*X + 4*X*X*X")
        #
        p2 = - p1
        p1 = Pol3D.Poly3D("0") # change p1
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
