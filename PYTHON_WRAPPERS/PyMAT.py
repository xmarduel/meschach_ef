#
# class Mat
#

import meschach
import meschach_adds

import sys

import PyVEC


class MatRow:
    """
    helper class be be able to type
        Mat MyMat(3,3)
        a = MyMat[2][1]
        MyMat[2][2] = b

        the term MyMat[2] return an instance of the class MatRow,
        -> the expression a = MyMat[2][1] calls the method __getitem__ of MatRow
        -> the expression MyMat[2][2] = b calls the method __setitem__ of MatRow
    """
    def __init__(self, FromMat, Row):
        self.FromMat = FromMat
        self.Row     = Row

    def __getitem__(self,key):
        return meschach.m_get_val(self.FromMat, self.Row, key)

    def __setitem__(self,key, val):
        meschach.m_set_val(self.FromMat, self.Row, key, val)


class Mat:
    """
    class for Dense Matrix : self.cmat is a MAT * structure
    """
    def __init__(self, m, n = 0):
        """
        setup object
        - m is a MAT*
        - m is a dimension
        """
        if Mat.is_cmat(m):
            self.cmat = m
        else:
            self.cmat = meschach.m_get(m,n)

    @classmethod
    def is_cmat(cls, cmat):
        return "MAT *" in str(cmat)

    def __del__(self):
        """ """
        self.free()

    #
    # Getting the data
    #
    # Get the dimension of the matrix
    #

    def get_m(self):
        return meschach_adds.m_dim1(self.cmat)
    def get_n(self):
        return meschach_adds.m_dim2(self.cmat)

    m = property( get_m, doc="1rst dim")
    n = property( get_n, doc="2nd dim")

    #
    # Getting the data
    #
    def __setitem__(self, key, ival):
        if not isinstance(key, int):
            raise TypeError("must be a integer")
        if not isinstance(ival, float):
            raise TypeError("must be a float")

        MatRowInstance = MatRow(self.cmat, key)
        return MatRowInstance

    def __getitem__(self, key):
        if not isinstance(key, int):
            raise TypeError("must be a integer")

        MatRowInstance = MatRow(self.cmat, key)
        return MatRowInstance

    #
    # Input - Output diagnostics
    #
    def out(self, file=None):
        """
        """
        if file == None:
            meschach.m_foutput(sys.stdout, self.cmat)
        else:
            meschach.m_foutput(file, self.cmat)

    def dump(self, file=None):
        """
        """
        if file == None:
            meschach.m_dump(sys.stdout,self.cmat)
        else:
            meschach.m_dump(file,self.cmat)

    def __str__(self): # as "out"
        f_tmp = open("tmp.dat", "w+")
        meschach.m_foutput(f_tmp, self.cmat)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str

    def __repr__(self): # as "dump"
        f_tmp = open("tmp.dat", "w+")
        meschach.m_dump(f_tmp, self.cmat)
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
        meschach.m_free(self.cmat)
        self.cmat = meschach.m_null()

    def resize(self, m, n):
        """
        """
        meschach.m_resize(self.cmat, m, n)

    #
    # Members functions
    #
    def ones(self):
        """
        """
        meschach.m_ones(self.cmat)

    def zero(self):
        """
        """
        meschach.m_zero(self.cmat)

    def rand(self):
        """
        """
        meschach.m_rand(self.cmat)

    def identity(self):
        """
        """
        meschach.m_ident(self.cmat)

    def __invert__(self):
        """
        transpose of the matrix At = ~A
        """
        ret = Mat(self.n, self.m)
        meschach.m_transp(self.cmat, ret.cmat)
        return ret

    def norm_frob(self):
        """
        """
        return meschach.m_norm_frob(self.cmat)

    #
    # operators creating temporary
    #
    def __add__(self, a):
        # a is a Mat
        if isinstance(a, Mat):
            ret = Mat(self.m, self.n)
            meschach.m_add(self.cmat, a.cmat, ret.cmat)
            return ret
        # a is a integer or a double
        elif isinstance(a, int) or isinstance(a, float):
            ret = Mat(self.m, self.n)
            meschach_adds.m_sadd(a, self.cmat, ret.cmat)
            return ret
        else:
            raise TypeError("wrong type")

    def __radd__(self, a):
        return self.__add__(a)

    def __sub__(self, a):
        # a is a Mat
        if isinstance(a, Mat):
            ret = Mat(self.m, self.n)
            meschach.m_sub(self.cmat, a.cmat, ret.cmat)
            return ret
        # a is a integer or a double
        elif isinstance(a, int) or isinstance(a, float):
            ret = Mat(self.m,self.n)
            meschach_adds.m_ssub(a, self.cmat, ret.cmat)
            return ret
        else:
            raise TypeError("wrong type")

    def __rsub__(self, a):
        return self.__sub__(a)

    def __mul__(self, a):
        # a is a Mat
        if isinstance(a, Mat):
            ret = Mat(self.m, a.n)
            meschach.m_mlt(self.cmat, a.cmat, ret.cmat)
            return ret
        # a is a Vec
        elif isinstance(a, PyVEC.Vec):
            ret = PyVEC.Vec(self.n)
            meschach.mv_mlt(self.cmat, a.cvec, ret.cvec)
            return ret
        # a is a integer or a double
        elif isinstance(a, int) or isinstance(a, float):
            ret = Mat(self.m, self.n)
            meschach.sm_mlt(a, self.cmat, ret.cmat)
            return ret
        else:
            raise TypeError("wrong type")

    def __rmul__(self, a):
        return self.__mul__(a)

    #
    # operator using and returning base object
    #
    def __iadd__(self, a):
        # a is a Mat
        if isinstance(a, Mat):
            meschach.m_add(self.cmat, a.cmat, self.cmat)
            return self
        # a is a integer or a double
        elif isinstance(a, int) or isinstance(a, float):
            meschach_adds.m_sadd(a, self.cmat, self.cmat)
            return self
        else:
            raise TypeError("wrong type")

    def __isub__(self, a):
        # a is a Mat
        if isinstance(a, Mat):
            meschach.m_sub(self.cmat, a.cmat, self.cmat)
            return self
        # a is a integer or a double
        elif isinstance(a, int) or isinstance(a, float):
            meschach_adds.m_ssub(a, self.cmat, self.cmat)
            return self
        else:
            raise TypeError("wrong type")

    def __imul__(self, a):
        # a is a Mat
        if isinstance(a, Mat):
            tmp = + self
            meschach.m_mlt(tmp.cmat, a.cmat, self.cmat)
            return self
        # a is a integer or a double
        elif isinstance(a, int) or isinstance(a, float):
            meschach.sm_mlt(a, self.cmat, self.cmat)
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
        ret = Mat(self.m, self.n)
        meschach.m_copy(self.cmat, ret.cmat)
        return ret

    # A = - B
    def __neg__(self):
        """
        """
        ret = Mat(self.m, self.n)
        meschach.m_copy(self.cmat, ret.cmat)
        meschach.sm_mlt(-1.0, ret.cmat, ret.cmat)
        return ret
