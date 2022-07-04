#
# class IMat
#
import meschach
import meschach_adds

import sys
import PyIVEC


class IMatRow:
    """
    helper class be be able to type
        IMat MyIMat(3,3)
        a = MyIMat[2][1]

        the term MyIMat[2] return an instance of the class IMatRow,
        and the expression a = MyIMat[2][1] calls the method __getattr__ of IMatRow
    """
    def __init__(self, FromIMat, Row):
        self.FromIMat = FromIMat
        self.Row      = Row

    def __getitem__(self,key):
        return meschach_adds.im_get_val(self.FromIMat, self.Row, key)

    def __setitem__(self,key, val):
        meschach_adds.im_set_val(self.FromIMat, self.Row, key, val)


class IMat:
    """
    class for Int Matrix : self.cimat is a IMAT * structure
    """
    def __init__(self, m, n):
        """
        setup self.cimat
        """
        self.cimat = meschach_adds.im_get(m, n)

    def resize(self,m,n):
        """
        """
        if hasattr(self, "cimat"):
            meschach_adds.im_resize(self.cimat, m, n)

    #
    # Get the dimension of the matrix
    #

    def _get_m(self):
        return meschach_adds.im_dim1(self.cimat)
    def _get_n(self):
        return meschach_adds.im_dim2(self.cimat)

    m = property( _get_m, doc="1rst dim")
    n = property( _get_n, doc="2nd dim")

    #
    # Getting the data
    #
    def __setitem__(self, key, ival):
        if not isinstance(key, int):
            raise TypeError("must be a integer")
        if not isinstance(ival, int):
            raise TypeError("must be a integer")

        IMatRowInstance = IMatRow(self.cimat, key)
        return IMatRowInstance

    def __getitem__(self,key):
        if not isinstance(key, int):
            raise TypeError("must be a integer")

        IMatRowInstance = IMatRow(self.cimat, key)
        return IMatRowInstance

    #
    # Input - Output diagnostics
    #
    def out(self,file=None):
        """
        """
        if file == None:
            meschach_adds.im_foutput(sys.stdout, self.cimat)
        else:
            meschach_adds.im_foutput(file, self.cimat)

    def dump(self,file=None):
        """
        """
        if file == None:
            meschach_adds.im_dump(sys.stdout, self.cimat)
        else:
            meschach_adds.im_dump(file, self.cimat)

    def __str__(self): # as "out"
        f_tmp = open("tmp.dat", "w+")
        meschach_adds.im_foutput(f_tmp, self.cimat)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str

    def __repr__(self): # as "dump"
        f_tmp = open("tmp.dat", "w+")
        meschach_adds.im_dump(f_tmp, self.cimat)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str

    #
    # Members functions
    #

    def ones(self):
        """
        """
        meschach_adds.im_ones(self.cimat)

    def zero(self):
        """
        """
        meschach_adds.im_zero(self.cimat)

    def rand(self):
        """
        """
        meschach_adds.im_rand(self.cimat)

    def identity(self):
        """
        """
        meschach_adds.im_ident(self.cimat)

    def __invert__(self):
        """
        transpose of the matrix At = ~A
        """
        ret = IMat(self.n, self.m)
        meschach_adds.im_transp(self.cimat, ret.cimat)
        return ret

    #
    # operators creating temporary
    #
    def __add__(self,a):
        # a is a IMat
        if isinstance(a, IMat):
            ret = IMat(self.m, self.n)
            meschach_adds.im_add(self.cimat, a.cimat, ret.cimat)
            return ret
        # a is a integer
        elif isinstance(a, int):
            ret = IMat(self.m,self.n)
            meschach_adds.im_sadd(a, self.cimat, ret.cimat)
            return ret
        else:
            raise TypeError("wrong type")

    def __radd__(self,a):
        return self.__add__(a)

    def __sub__(self,a):
        # a is a IMat
        if isinstance(a, IMat):
            ret = IMat(self.m, self.n)
            meschach_adds.im_sub(self.cimat, a.cimat, ret.cimat)
            return ret
        # a is a integer
        elif isinstance(a, int):
            ret = IMat(self.m,self.n)
            meschach_adds.im_ssub(a, self.cimat, ret.cimat)
            return ret
        else:
            raise TypeError("wrong type")

    def __rsub__(self,a):
        return self.__sub__(a)

    def __mul__(self, a):
        # a is a IMat
        if isinstance(a, IMat):
            ret = IMat(self.m, a.n)
            meschach_adds.im_mlt(self.cimat, a.cimat, ret.cimat)
            return ret
        # a is a IVec
        if isinstance(a, PyIVEC.IVec):
            ret = PyIVEC.IVec(self.n)
            meschach_adds.imv_mlt(self.cimat, a.cimat, ret.civec)
            return ret
        # a is a integer
        elif isinstance(a, int):
            ret = IMat(self.m, self.n)
            meschach_adds.im_smlt(self.cimat, a, ret.cimat)
            return ret
        else:
            raise TypeError("wrong type")

    def __rmul__(self,a):
        return self.__mul__(a)

    #
    # operator using and returning base object
    #
    def __iadd__(self,a):
        # a is a IMat
        if isinstance(a, IMat):
            meschach_adds.im_add(self.cimat, a.cimat, self.cimat)
            return self
        # a is a integer
        elif isinstance(a, int):
            meschach_adds.im_sadd(a, self.cimat, self.cimat)
            return self
        else:
            raise TypeError("wrong type")

    def __isub__(self,a):
        # a is a IMat
        if isinstance(a, IMat):
            meschach_adds.im_sub(self.cimat, a.cimat, self.cimat)
            return self
        # a is a integer
        elif ( isinstance(a, int) ):
            meschach_adds.im_ssub(a, self.cimat, self.cimat)
            return self
        else:
            raise TypeError("wrong type")

    def __imul__(self,a):
        # a is a IMat
        if ( isinstance(a, IMat) ):
            tmp = + self
            meschach_adds.im_mlt(tmp.cimat, a.cimat, self.cimat)
            return self
        # a is a integer
        elif isinstance(a, int):
            meschach_adds.im_smlt(self.cimat, a, self.cimat)
            return self
        else:
            raise TypeError("wrong type")

    #
    # assignation operator
    #
    def __pos__(self):
        """
        to clone a matrix : m1 = + m2   --- memory is not shared ---
        """
        ret = IMat(self.m, self.n)
        meschach_adds.im_copy(self.cimat, ret.cimat)
        return ret

    # A = - B
    def __neg__(self):
        """
        """
        ret = IMat(self.m, self.n)
        meschach_adds.im_copy(self.cimat, ret.cimat)
        meschach_adds.im_smlt(ret.cimat, -1, ret.cimat)
        return ret
