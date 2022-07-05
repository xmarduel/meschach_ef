#
# class IVec
#

import sys

import meschach
import meschach_adds


class IVec:
    """
    class for IntVector : self.civec is a IVEC * structure
    """
    def __init__(self, m):
        """
        setup self.civec
        """
        self.civec = meschach.iv_get(m)

    #
    #  Get the dimension of the matrix
    #
    def _get_m(self):
        return meschach_adds.iv_dim(self.civec)

    m = property( _get_m , doc="dim")


    def __len__(self):
        return meschach_adds.iv_dim(self.civec)

    # Getting the data

    def __setitem__(self, key, ival):
        if not isinstance(key, int):
            raise TypeError("must be a integer")
        if not isinstance(ival, int):
            raise TypeError("must be a integer")
        return meschach.iv_set_val(self.civec, key, ival)

    def __getitem__(self,key):
        if not isinstance(key, int):
            raise TypeError("must be a integer")
        return meschach.iv_get_val(self.civec, key)

    #
    # Input - Output diagnostics
    #
    def out(self,file=sys.stdout):
        """
        """
        meschach.iv_foutput(file, self.civec)

    def dump(self,file=None):
        """
        """
        meschach.iv_dump(file, self.civec)

    def __str__(self): # as "out"
        f_tmp = open("tmp.dat", "w+")
        meschach.iv_foutput(f_tmp, self.civec)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str

    def __repr__(self): # as "dump"
        f_tmp = open("tmp.dat", "w+")
        meschach.iv_dump(f_tmp, self.civec)
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
        meschach.iv_free(self.civec)
        self.civec = meschach_adds.iv_null()

    def resize(self,m):
        """
        """
        meschach_adds.iv_resize(self.civec, m)

    #
    # Members functions
    #
    def ones(self):
        """
        """
        meschach_adds.iv_ones(self.civec)

    def zero(self):
        """
        """
        meschach.iv_zero(self.civec)

    def rand(self):
        """
        """
        meschach_adds.iv_rand(self.civec)

    #
    # operators creating temporary
    #
    def __add__(self, a):
        # a is a IVec
        if isinstance(a, IVec):
            ret = IVec(self.m)
            meschach.iv_add(self.civec, a.civec, ret.civec)
            return ret
        # a is a integer
        elif isinstance(a, int):
            ret = IVec(self.m)
            meschach_adds.iv_sadd(a, self.civec, ret.civec)
            return ret
        # bad argument
        else:
            raise TypeError("wrong type")

    def __radd__(self, a):
        return self.__add__(a)

    def __sub__(self, a):
        # a is a IVec
        if isinstance(a, IVec):
            ret = IVec(self.m)
            meschach.iv_sub(self.civec, a.civec, ret.civec)
            return ret
        # a is a integer
        elif isinstance(a, int):
            ret = IVec(self.m)
            meschach_adds.iv_ssub(a, self.civec, ret.civec)
            return ret
        # bad argument
        else:
            raise TypeError("wrong type")

    def __rsub__(self, a):
        return self.__sub__(a)

    def __mul__(self, a):
        # a is a IVec
        #if ( isinstance(a, IVec) ):
        #    ret = IVec(self.dim)
        #    ret = in_prod(self.civec, a.civec)
        #    return ret
        # a is a integer or a double
        if isinstance(a, int):
            ret = IVec(self.m)
            meschach_adds.iv_smlt(a, self.civec, ret.civec)
            return ret
        else:
            raise TypeError("wrong type")

    def __rmul__(self, a):
        return self.__mul__(a)

    #
    # operator using and returning base object
    #
    def __iadd__(self, a):
        # a is a IVec
        if isinstance(a, IVec):
            meschach.iv_add(self.civec, a.civec, self.civec)
            return self
        # a is a integer
        elif isinstance(a, int):
            meschach_adds.iv_sadd(a, self.civec, self.civec)
            return self
        # bad argument
        else:
            raise TypeError("wrong type")

    def __isub__(self, a):
        # a is a IVec
        if isinstance(a, IVec):
            meschach.iv_sub(self.civec, a.civec, self.civec)
            return self
        # a is a integer
        elif isinstance(a, int):
            meschach_adds.iv_ssub(a, self.civec, self.civec)
            return self
        # bad argument
        else:
            raise TypeError("wrong type")

    def __imul__(self, a):
        # a is a integer
        if isinstance(a, int):
            meschach_adds.iv_smlt(a, self.civec, self.civec)
            return self
        # bad argument
        else:
            raise TypeError("wrong type")

    #
    # assignation operator :

    # A = + B
    def __pos__(self): # unary op.
        """
        to clone a vector : v1 = + v2   --- memory is not shared ---
        """
        ret = IVec(self.m)
        meschach.iv_copy(self.civec, ret.civec)
        return ret

    # A = - B
    def __neg__(self): # unary op.
        """
        get the negativ of a vector -- a vector is created --
        """
        ret = IVec(self.m)
        meschach.iv_copy(self.civec, ret.civec)
        meschach_adds.iv_smlt(-1, ret.civec, ret.civec)
        return ret
