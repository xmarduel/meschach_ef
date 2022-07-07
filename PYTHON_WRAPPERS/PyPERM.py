#
# class Perm
#

import meschach
import meschach_adds

import sys


class Perm(object):
    """
    class for Permutation : self.cperm is a PERM * structure
    """
    def __init__(self,m):
        """
        setup self.cperm
        """
        self.cperm = meschach.px_get(m)

    def __del__(self):
        """ """
        self.free()

    #
    # getting the data
    #
    def _get_size(self):
        return meschach_adds.px_size(self.cperm)

    size = property( _get_size, doc="size")

    def __len__(self):
        return meschach_adds.px_size(self.cperm)

    def __setitem__(self, key, ival):
        raise TypeError("use swap to manipulate a Permutation")

    def __getitem__(self, key):
        return meschach.px_get_val(self.cperm, key)

    #
    # Input - Output diagnostics
    #

    def out(self, file=None):
        """
        """
        if file == None:
            meschach.px_foutput(sys.stdout, self.cperm)
        else:
            meschach.px_foutput(file, self.cperm)

    def dump(self,file=None):
        """
        """
        if file == None:
            meschach.px_dump(sys.stdout, self.cperm)
        else:
            meschach.px_dump(file, self.cperm)

    def __str__(self): # as "out"
        f_tmp = open("tmp.dat", "w+")
        meschach.px_foutput(f_tmp, self.cperm)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str

    def __repr__(self): # as "dump"
        f_tmp = open("tmp.dat", "w+")
        meschach.px_dump(f_tmp, self.cperm)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str

    #
    # playing with storage
    #
    def free(self):
        """
        """
        meschach.px_free(self.cperm)
        self.cperm = meschach.px_null()

    def resize(self, m):
        """
        """
        meschach.px_resize(self.cperm, m)

    #
    # playing with storage
    #
    def free(self):
        """
        """
        meschach.m_free(self.cperm)
        self.cperm = meschach.px_null()

    #
    # Members functions
    #

    def ident(self):
        """
        """
        meschach.px_ident(self.cperm)

    def swap(self, i, j):
        """
        """
        meschach.px_transp(self.cperm, i, j)

    def __mul__(self, a):
        # a is a Perm
        if isinstance(a, Perm):
            ret = Perm(self.size)
            meschach.px_mlt(self.cperm, a.cperm, ret.cperm)
        else:
            raise TypeError("wrong type")

        return ret

    #
    # operator using and returning base object
    #
    def __imul__(self, a):
        # a is a integer or a double
        if isinstance(a, Perm):
            ret = Perm(self.size)
            meschach.px_mlt(self.cperm, a.cperm, ret.cperm) # not in-situ
            return ret
        else:
            raise TypeError("wrong type")

    #
    # assignation operator : with __pos__
    #

    # A = + B
    def __pos__(self): # unary op.
        """
        to clone a permutation
        """
        ret = Perm(self.size)
        meschach.px_copy(self.cperm, ret.cperm)
        return ret

    # A = - B
    def __neg__(self): # unary op.
        """
        """
        ret = Perm(self.size)
        meschach.px_inv(self.cperm, ret.cperm)
        return ret
