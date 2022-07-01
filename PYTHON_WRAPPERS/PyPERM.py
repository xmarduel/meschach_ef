#
# class Perm
#

import meschach
import meschach_adds

import sys


class Perm(object):
    """
    class for Permutation : self.this is a PERM * structure
    """
    def __init__(self,m):
        """
        setup self.this
        """
        try:
            cperm = meschach.px_get(m)
            self.__dict__["this"]  = cperm
        except:
            raise IndexError("negative argument for <Perm> initialization")

    def __del__(self):
        """
        free mem of self.this
        """
        if hasattr(self,"this"):
            meschach.px_free(self.this)
        self.__dict__["this"] = meschach.px_null()

    #
    # getting the data
    #
    def _get_size(self):
        return meschach_adds.px_size(self)

    size = property( _get_size, doc="size")


    def __len__(self):
        return meschach_adds.px_size(self)

    def __setattr__(self,name, value):
        if ( name == "this" ):
            raise AttributeError("not allowed to change self.this in this way")
        else:
            pass
            #raise AttributeError, "not allowed to add an attribute to an IMat instance"

    def __setitem__(self,key,ival):
        raise TypeError("use swap to manipulate  a Permutation")

    def __getitem__(self,key):
        return meschach.px_get_val(self.this,key)

    #
    # Input - Output diagnostics
    #
    def out(self,file=None):
        """
        """
        if (file==None):
            meschach.px_foutput(stdout,self.this)
        else:
            meschach.px_foutput(file,self.this)

    def dump(self,file=None):
        """
        """
        if (file==None):
            meschach.px_dump(stdout,self.this)
        else:
            meschach.px_dump(file,self.this)

    def __str__(self): # as "out"
        f_tmp = open("tmp.dat", "w+")
        meschach.px_foutput(f_tmp, self.this)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str
    #
    def __repr__(self): # as "dump"
        f_tmp = open("tmp.dat", "w+")
        meschach.px_dump(f_tmp, self.this)
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
        meschach.px_free(self.this)
        self.__dict__["this"] = meschach.px_null()

    def resize(self,m):
        """
        """
        meschach.px_resize(self.this, m)

    #
    # Members functions
    #
    def ident(self):
        """
        """
        meschach.px_ident(self.this)

    def swap(self, i, j):
        """
        """
        meschach.px_transp(self.this, i, j)

    def __mul__(self,a):
        # a is a Perm
        if ( isinstance(a, Perm) ):
            ret = Perm(self.size)
            meschach.px_mlt(self.this, a.this, ret.this)
        else:
            raise TypeError("wrong type")

        return ret

    #
    # operator using and returning base object
    #
    def __imul__(self,a):
        # a is a integer or a double
        if ( isinstance(a, Perm) ):
            ret = Perm(self.size)
            meschach.px_mlt(self.this, a.this, ret.this) # not in-situ
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
        meschach.px_copy(self.this, ret.this)
        return ret

    # A = - B
    def __neg__(self): # unary op.
        """
        """
        ret = Perm(self.size)
        meschach.px_inv(self.this, ret.this)
        return ret
