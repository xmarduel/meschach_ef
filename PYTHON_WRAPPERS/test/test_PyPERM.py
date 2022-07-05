#! /usr/bin/env python

""" This module tests all the methods of the Perm class (PyPERM.py) """

import unittest
import PyPERM


class PermTest(unittest.TestCase):

    def testInit(self):
        px1 = PyPERM.Perm(5)

    def test___del__(self):
        px1 = PyPERM.Perm(4)
        del px1

    #
    # Getting the data
    #
    def test___len__(self):
        N = 10
        p1 = PyPERM.Perm(N)
        self.assertEqual(len(p1), N)

    def test___setitem__(self):
        p1 = PyPERM.Perm(3)
        p1.ident()

        self.assertRaises(TypeError, 'p1[0] = 0')

    def test___getitem__(self):
        p1 = PyPERM.Perm(3)
        p1.ident()
        p1.swap(1,2)

        self.assertEqual(p1[0], 0)
        self.assertEqual(p1[1], 2)
        self.assertEqual(p1[2], 1)

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
        p1 = PyPERM.Perm(3)
        self.assertEqual(p1.__str__(), 'Permutation: size: 3\n0->0 1->1 2->2 \n')
    #
    # playing with storage
    #
    #def free(self):
    #    px_free(self.this)
    #    self.this = px_null()
    #
    #def resize(self,m):
    #    px_resize(self.this, m)
    #
    #
    # Members functions
    #
    def test_ident(self):
        p1 = PyPERM.Perm(3)
        p1.ident()
        self.assertEqual(p1.__str__(), 'Permutation: size: 3\n0->0 1->1 2->2 \n')

    def test_swap(self):
        p1 = PyPERM.Perm(3)
        p1.ident()
        p1.swap(1,2)
        self.assertEqual(p1.__str__(), 'Permutation: size: 3\n0->0 1->2 2->1 \n')

    #
    # operators creating temporary
    #
    def test___mul__(self):
        p1 = PyPERM.Perm(3)
        p2 = PyPERM.Perm(3)

        p1.ident()
        p2.ident()

        p1.swap(1, 2)
        p2.swap(2, 1)

        p3 = p1 * p2
        self.assertEqual(p3.__str__(), 'Permutation: size: 3\n0->0 1->1 2->2 \n')

    #
    # operator using and returning base object
    #
    def test___imul__(self):
        p1 = PyPERM.Perm(3)
        p2 = PyPERM.Perm(3)

        p1.ident()
        p2.ident()

        p2.swap(1,2)

        p1 *= p2
        self.assertEqual(p1.__str__(), 'Permutation: size: 3\n0->0 1->2 2->1 \n')

    #
    # assignation operator : with __pos__

    # A = + B
    def test___pos__(self):
        p1 = PyPERM.Perm(3)
        p2 = PyPERM.Perm(3)

        p1.ident()
        p2.ident()

        p2 = + p1
        p1.swap(1,2) # change p1
        self.assertEqual(p2.__str__(), 'Permutation: size: 3\n0->0 1->1 2->2 \n')

    # A = - B
    def test___neg__(self):
        p1 = PyPERM.Perm(3)
        p2 = PyPERM.Perm(3)

        p1.ident()
        p2.ident()

        p2 = - p1
        p1.swap(1,2) # change p1
        self.assertEqual(p2.__str__(), 'Permutation: size: 3\n0->0 1->1 2->2 \n')



if __name__ == '__main__' :
    unittest.main()
